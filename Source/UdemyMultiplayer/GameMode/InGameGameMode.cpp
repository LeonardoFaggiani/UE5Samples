// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "UdemyMultiplayer/InGamePlayerController.h"
#include "UdemyMultiplayer/UdemyMultiplayerPlayerState.h"



AInGameGameMode::AInGameGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UWorld* World = GetWorld();

	if (IsValid(World))
	{
		UGameInstance* GameInstance = World->GetGameInstance();

		if (IsValid(GameInstance)) {
			UdemyMultiplayerGameInstance = Cast<UUdemyMultiplayerGameInstance>(GameInstance);
		}
	}
}

void AInGameGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (HasAuthority())
	{
		//if the joining player is a lobby player controller, add him to a list of connected Players
		if (NewPlayer) {
			AInGamePlayerController* InGamePlayerController = Cast<AInGamePlayerController>(NewPlayer);

			this->AllPlayerControllers.Add(InGamePlayerController);

			if (IsValid(UdemyMultiplayerGameInstance))
				this->Server_RespawnPlayer(InGamePlayerController);			
		}
	}
}

void AInGameGameMode::Logout(AController* Exiting)
{

}

void AInGameGameMode::Server_RespawnPlayer_Implementation(AInGamePlayerController* InGamePlayerController)
{
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	int32 InPlayerIndex = this->AllPlayerControllers.IndexOfByKey(InGamePlayerController);

	FTransform Transform = this->GetTransformOfPlayerStartByIndex(InPlayerIndex);
	FInGamePlayerInfo* InGamePlayerInfo = this->GetPlayerInfoByIndex(InPlayerIndex);

	AUdemyMultiplayerCharacter* UdemyMultiplayerCharacter = Cast<AUdemyMultiplayerCharacter>(GetWorld()->SpawnActor<AUdemyMultiplayerCharacter>(InGamePlayerInfo->HeroeSelected, Transform, params));

	FTimerHandle MemberTimerHandle;
	FTimerDelegate TimerDel;

	TimerDel.BindUFunction(this, FName("Server_SetOverheadPlayer"), true, FString("FFFFFF"));

	GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, TimerDel, 0.1f, false);

	InGamePlayerController->SetCurrentCharacter(UdemyMultiplayerCharacter);
	InGamePlayerController->Possess(UdemyMultiplayerCharacter);
}

void AInGameGameMode::Server_UpdatePlayerName_Implementation()
{
	for (AInGamePlayerController* InGamePlayerController : this->AllPlayerControllers)
		this->UpdatePlayerName(InGamePlayerController);
}

void AInGameGameMode::Server_SetOverheadPlayer_Implementation(bool bIsHiddenIcon, const FString& ColorName)
{
    for (AInGamePlayerController* InGamePlayerController : this->AllPlayerControllers) {

        AUdemyMultiplayerCharacter* UdemyMultiplayerCharacter = InGamePlayerController->GetCurrentCharacter();

        if (IsValid(UdemyMultiplayerCharacter))
            UdemyMultiplayerCharacter->Multi_SetIconAndColorOverheadWidget(bIsHiddenIcon, ColorName);
    }
	
}

void AInGameGameMode::UpdatePlayerName(AInGamePlayerController* InGamePlayerController)
{
	AUdemyMultiplayerPlayerState* UdemyMultiplayerPlayerState = InGamePlayerController->GetPlayerState<AUdemyMultiplayerPlayerState>();

	AUdemyMultiplayerCharacter* UdemyMultiplayerCharacter = InGamePlayerController->GetCurrentCharacter();

	if (IsValid(UdemyMultiplayerCharacter) && IsValid(UdemyMultiplayerPlayerState))
		UdemyMultiplayerCharacter->Multi_SetPlayerName(UdemyMultiplayerPlayerState->GetPlayerName());				
}


TArray<AActor*> AInGameGameMode::GetAllPlayersStart()
{
	TArray<AActor*> AllPlayersStart;

	this->PlayerStart = APlayerStart::StaticClass();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), this->PlayerStart, AllPlayersStart);

	return AllPlayersStart;
}

FTransform AInGameGameMode::GetTransformOfPlayerStartByIndex(int32 InPlayerIndex)
{
	TArray<AActor*> PlayersStart = this->GetAllPlayersStart();

	FTransform Transform = PlayersStart[InPlayerIndex]->GetTransform();

	return Transform;
}

FInGamePlayerInfo* AInGameGameMode::GetPlayerInfoByIndex(int32 InPlayerIndex)
{
	auto GetPlayerByIndex = [InPlayerIndex](const FInGamePlayerInfo& Player) {
		return Player.PlayerIndex == InPlayerIndex;
	};

	FInGamePlayerInfo* InGamePlayerInfo = this->UdemyMultiplayerGameInstance->InGamePlayersInfo.FindByPredicate(GetPlayerByIndex);

	return InGamePlayerInfo;
}

void AInGameGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInGameGameMode, ConnectedPlayers);
}

