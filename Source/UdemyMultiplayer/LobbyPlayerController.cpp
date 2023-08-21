// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "GameMode/LobbyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ALobbyPlayerController::ALobbyPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UWorld* World = this->GetWorld();

	if (IsValid(World))
	{
		AGameModeBase* GameMode = UGameplayStatics::GetGameMode(World);

		if (IsValid(GameMode))
			LobbyGameMode = Cast<ALobbyGameMode>(GameMode);

		UGameInstance* GameInstance = World->GetGameInstance();

		if (IsValid(GameInstance)) {
			UdemyMultiplayerGameInstance = Cast<UUdemyMultiplayerGameInstance>(GameInstance);
		}
	}
}

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

#pragma region Server

void ALobbyPlayerController::Server_CallUpdate_Implementation(const FLobbyPlayerInfo& PlayerInfo)
{
	PlayerSettings = PlayerInfo;

	if (IsValid(LobbyGameMode)) {
		LobbyGameMode->Server_SwapCharacter(this, PlayerSettings.HeroeSelected, PlayerSettings.bPlayerReadyState);
		LobbyGameMode->Server_EveryoneUpdate();
	}
}

void ALobbyPlayerController::Server_NotifyPlayerStatus_Implementation(const FLobbyPlayerInfo& PlayerInfo)
{
	PlayerSettings = PlayerInfo;

	if (IsValid(LobbyGameMode)) {
		LobbyGameMode->Server_EveryoneUpdate();
	}
}

#pragma endregion Server

#pragma region Client

void ALobbyPlayerController::Client_SetupLobbyMenu_Implementation(const FString& ServerName)
{
	if (!ensure(LobbyClass != nullptr)) return;

	this->Lobby = CreateWidget<ULobby>(this, LobbyClass);

	if (!ensure(this->Lobby != nullptr)) return;

	this->Lobby->SetServerName(ServerName);
	this->Lobby->Setup();
}

void ALobbyPlayerController::Client_UpdateLobbySettings_Implementation(UTexture2D* MapImage, const FString& MapName)
{
	if (!IsValid(this->Lobby)) return;

	this->Lobby->SetMap(MapImage, MapName);
}

void ALobbyPlayerController::Client_UpdateNumberOfPlayers_Implementation(int32 CurrentPlayers, int32 MaxPlayers)
{
	if (!IsValid(this->Lobby)) return;

	TArray<FStringFormatArg> CurrentAndMaxPlayers;
	CurrentAndMaxPlayers.Add(FStringFormatArg(CurrentPlayers));
	CurrentAndMaxPlayers.Add(FStringFormatArg(MaxPlayers));

	FString CurrentAndMaxPlayersFormat = FString::Format(TEXT("{0}/{1} players"), CurrentAndMaxPlayers);

	if (IsValid(LobbyGameMode))
		this->Lobby->SetEnablePlayButton(LobbyGameMode->IsAllPlayerReady());
	
	this->Lobby->SetCurrentPlayersFormat(CurrentAndMaxPlayersFormat);
}

void ALobbyPlayerController::Client_AssignHeroeToPlayer_Implementation(TSubclassOf<AUdemyMultiplayerCharacter> HeroeClass)
{
	if (this->PlayerSettings.bPlayerReadyState)
		return;

	this->PlayerSettings.HeroeSelected = HeroeClass;

	this->Server_CallUpdate(this->PlayerSettings);
}

void ALobbyPlayerController::Client_ShowLoadingScreen_Implementation() 
{
	this->UdemyMultiplayerGameInstance->ShowLoadingScreen(true);
}

void ALobbyPlayerController::Client_SetViewTargetSpot_Implementation()
{
	this->ViewTarget = GetActorByName("ViewTargetSpot");

	this->SetViewTargetWithBlend(this->ViewTarget);
}

void ALobbyPlayerController::Client_SwitchToLobbyMode_Implementation()
{
	if (!IsValid(this->Lobby)) return;

	this->Lobby->SetHiddenHeroesButton(false);
}

#pragma endregion Client

AActor* ALobbyPlayerController::GetActorByName(FString InActorName)
{
	TArray<AActor*> ActorsInScene{};

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), ActorsInScene);

	if (ActorsInScene.Num() > 0)
	{
		for (AActor* actor : ActorsInScene)
		{
			if (actor == nullptr)
				return nullptr;

			if (actor->GetActorNameOrLabel() == InActorName)
				return actor;
		}
	}

	return nullptr;
}

void ALobbyPlayerController::SetCurrentCharacter(AUdemyMultiplayerCharacter* currentCharacter)
{
	this->CurrentCharacter = currentCharacter;
}

AUdemyMultiplayerCharacter* ALobbyPlayerController::GetCurrentCharacter()
{
	return this->CurrentCharacter;
}

void ALobbyPlayerController::UpdateReadyState()
{
	AUdemyMultiplayerCharacter* UdemyMultiplayerCharacter = this->GetCurrentCharacter();

	if (IsValid(UdemyMultiplayerCharacter))
	{
		UdemyMultiplayerCharacter->SetIsReady(this->PlayerSettings.bPlayerReadyState);
		UdemyMultiplayerCharacter->OnRep_ReadyStateUpdated();
	}
}

void ALobbyPlayerController::SetPlayerSpot(ALobbyPlayerSpot* playerSpot)
{
	this->PlayerSpot = playerSpot;
}

ALobbyPlayerSpot* ALobbyPlayerController::GetPlayerSpot()
{
	return this->PlayerSpot;
}

void ALobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerController, PlayerSettings);
	DOREPLIFETIME(ALobbyPlayerController, PlayerSpot);
}