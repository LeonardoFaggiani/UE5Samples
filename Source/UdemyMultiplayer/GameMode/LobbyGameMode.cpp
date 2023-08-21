// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "../LobbyPlayerController.h"
#include "../UdemyMultiplayerGameInstance.h"
#include "../UdemyMultiplayerCharacter.h"
#include "../Menu/LobbyPlayerSpot.h"
#include "../UdemyMultiplayerPlayerState.h"


ALobbyGameMode::ALobbyGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{	
	PlayerControllerClass = ALobbyPlayerController::StaticClass();	

	UWorld* World = GetWorld();

    if (IsValid(World))
    {
        UGameInstance* GameInstance = World->GetGameInstance();

		if (IsValid(GameInstance)) {
			UdemyMultiplayerGameInstance = Cast<UUdemyMultiplayerGameInstance>(GameInstance);
			DefaultCharacterChampion = UdemyMultiplayerGameInstance->GetHeroeByName("Warrior");
		}
    }
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer) {

    Super::PostLogin(NewPlayer);

    if (HasAuthority())
    {
        //if the joining player is a lobby player controller, add him to a list of connected Players
        if (NewPlayer) {
            ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(NewPlayer);

            this->AllPlayerControllers.Add(LobbyPlayerController);

            if (UdemyMultiplayerGameInstance != nullptr) {

                MaxPlayers = UdemyMultiplayerGameInstance->MaxPlayers;

                FConfigurationMaps* DefaultMap = UdemyMultiplayerGameInstance->ConfigurationMaps.Find(TEXT("LobbyMap"));

                UTexture2D* mapImage = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *DefaultMap->ImagePath));
                
				LobbyPlayerController->Client_SetupLobbyMenu(UdemyMultiplayerGameInstance->ServerName);

				LobbyPlayerController->Client_UpdateLobbySettings(mapImage, *DefaultMap->Name);

				Server_SpawnLobbyPlayerSpot();

                Server_RespawnPlayer(LobbyPlayerController);

				Server_EveryoneUpdate();	

				FTimerHandle MemberTimerHandle;

				GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, this, &ThisClass::Server_SetViewTargetSpot, 0.2f, false);
            }
        }
    }
}

void ALobbyGameMode::Logout(AController* Exiting) {

	Super::Logout(Exiting);

	ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(Exiting);

	if (LobbyPlayerController)	
		this->AllPlayerControllers.Remove(LobbyPlayerController);	

	Server_EveryoneUpdate();
}

bool ALobbyGameMode::IsAllPlayerReady()
{
	for (FLobbyPlayerInfo PlayerInfo : this->ConnectedPlayers)
	{
		if (!PlayerInfo.bPlayerReadyState)
			return false;
	}

	return true;
}

void ALobbyGameMode::Server_SwapCharacter_Implementation(APlayerController* PlayerController, TSubclassOf<AUdemyMultiplayerCharacter> InHeroeSelected, bool bChangeStatus)
{
    if (!bChangeStatus) {
		
		ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(PlayerController);
		
		this->DefaultCharacterChampion = InHeroeSelected;
		
		this->DestroyCharacterSelectedIfExits(LobbyPlayerController);       
    }
}

void ALobbyGameMode::Server_UpdateGameSettings_Implementation(UTexture2D* mapImage, const FString& mapName)
{
	this->MapImage = mapImage;
	this->MapName = mapName;

	for (ALobbyPlayerController* PlayerController : this->AllPlayerControllers)
	{
		PlayerController->Client_UpdateLobbySettings(this->MapImage, this->MapName);
	}
}

void ALobbyGameMode::Server_SetViewTargetSpot_Implementation()
{
	for (ALobbyPlayerController* PlayerController : this->AllPlayerControllers)
	{
		PlayerController->Client_SetViewTargetSpot();
	}
}

void ALobbyGameMode::Server_SpawnLobbyPlayerSpot_Implementation()
{
	FPlayerSpot* PlayerSpotByIndex = GetPlayerSpotForPlayerConnected();

	if (PlayerSpotByIndex != nullptr)
	{
		FActorSpawnParameters params;

		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		for (ALobbyPlayerController* PlayerController : this->AllPlayerControllers)
		{
			ALobbyPlayerSpot* LobbyPlayerSpot = PlayerController->GetPlayerSpot();

			if (!IsValid(LobbyPlayerSpot)) {

				ALobbyPlayerSpot* LobbyPlayerSpotSpawned = Cast<ALobbyPlayerSpot>(GetWorld()->SpawnActor<ALobbyPlayerSpot>(LobbyPlayerSpotClass, PlayerSpotByIndex->Location, PlayerSpotByIndex->Rotation, params));

				PlayerController->SetPlayerSpot(LobbyPlayerSpotSpawned);
			}
		}
	}
}

void ALobbyGameMode::Server_RespawnPlayer_Implementation(ALobbyPlayerController* LobbyPlayerController)
{
	this->DestroyCharacterSelectedIfExits(LobbyPlayerController);
}

void ALobbyGameMode::Server_UpdatePlayerName_Implementation()
{
    for (ALobbyPlayerController* LobbyPlayerController : this->AllPlayerControllers)
    {
        AUdemyMultiplayerPlayerState* UdemyMultiplayerPlayerState = LobbyPlayerController->GetPlayerState<AUdemyMultiplayerPlayerState>();

		AUdemyMultiplayerCharacter* UdemyMultiplayerCharacter = LobbyPlayerController->GetCurrentCharacter();

		if (IsValid(UdemyMultiplayerCharacter) && IsValid(UdemyMultiplayerPlayerState)) {
			LobbyPlayerController->PlayerSettings.PlayerName = UdemyMultiplayerPlayerState->GetPlayerName();
			UdemyMultiplayerCharacter->Multi_SetPlayerName(LobbyPlayerController->PlayerSettings.PlayerName);
		}		
    }
}

void ALobbyGameMode::Server_EveryoneUpdate_Implementation() 
{
	this->CurrentPlayers = this->AllPlayerControllers.Num();

	if (this->CurrentPlayers > 0)	
		this->ConnectedPlayers.Empty();

	for (ALobbyPlayerController* LobbyPlayerController : this->AllPlayerControllers)
	{
		this->ConnectedPlayers.Add(LobbyPlayerController->PlayerSettings);
	}

	for (ALobbyPlayerController* LobbyPlayerController : this->AllPlayerControllers)
	{		
		LobbyPlayerController->Client_UpdateNumberOfPlayers(this->CurrentPlayers, this->MaxPlayers);

		AUdemyMultiplayerCharacter* UdemyMultiplayerCharacter = LobbyPlayerController->GetCurrentCharacter();

		if (IsValid(UdemyMultiplayerCharacter)) {
			UdemyMultiplayerCharacter->SetIsReady(LobbyPlayerController->PlayerSettings.bPlayerReadyState);
			UdemyMultiplayerCharacter->OnRep_ReadyStateUpdated();
		}
	}
}

void ALobbyGameMode::LaunchTheGame() 
{
	UWorld* World = GetWorld();

	const FString FullMapPath = FString(TEXT("/Game/ThirdPerson/Maps/{0}?listen"));

	FString MapToTravel = FString::Format(*FullMapPath, { MapName });

	if (World) {
		//bUseSeamlessTravel = true;		
		World->ServerTravel(FString(MapToTravel));
	}		
}

void ALobbyGameMode::SpawnCharacterOnPlayerSpot(ALobbyPlayerController* LobbyPlayerController)
{
	ALobbyPlayerSpot* LobbyPlayerSpot = LobbyPlayerController->GetPlayerSpot();

	if (!IsValid(LobbyPlayerSpot))
		return;

	FActorSpawnParameters params;

	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AUdemyMultiplayerCharacter* SpawnCharacter = Cast<AUdemyMultiplayerCharacter>(GetWorld()->SpawnActor<AUdemyMultiplayerCharacter>(DefaultCharacterChampion, LobbyPlayerSpot->GetActorTransform(), params));

	FTransform ActorTransform = LobbyPlayerSpot->GetActorTransform();
	
	FVector	Location = ActorTransform.GetLocation();

	Location[2] = 90;

	ActorTransform.SetLocation(Location);

	SpawnCharacter->SetActorTransform(ActorTransform);

	LobbyPlayerController->SetCurrentCharacter(SpawnCharacter);
}

void ALobbyGameMode::DestroyCharacterSelectedIfExits(ALobbyPlayerController* LobbyPlayerController)
{
    AUdemyMultiplayerCharacter* UdemyMultiplayerCharacter = LobbyPlayerController->GetCurrentCharacter();

	if (IsValid(UdemyMultiplayerCharacter))
		UdemyMultiplayerCharacter->Destroy();
	
	this->SpawnCharacterOnPlayerSpot(LobbyPlayerController);
}

FPlayerSpot* ALobbyGameMode::GetPlayerSpotForPlayerConnected()
{
	FPlayerSpot* PlayerSpotIndex = UdemyMultiplayerGameInstance->ConfigurationPlayerSpot.Find(this->AllPlayerControllers.Num());

	return PlayerSpotIndex;
}

void ALobbyGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGameMode, AllPlayerControllers);
	DOREPLIFETIME(ALobbyGameMode, MapImage);
	DOREPLIFETIME(ALobbyGameMode, MapName);
	DOREPLIFETIME(ALobbyGameMode, CurrentPlayers);
	DOREPLIFETIME(ALobbyGameMode, MaxPlayers);
	DOREPLIFETIME(ALobbyGameMode, ConnectedPlayers);
}