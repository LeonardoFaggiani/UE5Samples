// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "../UdemyMultiplayerPlayerState.h"
#include "../LobbyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Net/UnrealNetwork.h"
#include "../UdemyMultiplayerGameInstance.h"
#include "../UdemyMultiplayerCharacter.h"
#include "../Menu/LobbyPlayerSpot.h"
#include "GameFramework/GameStateBase.h"


ALobbyGameMode::ALobbyGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<AUdemyMultiplayerCharacter> WarriorBPClass(TEXT("/Game/Blueprints/Characters/BP_Warrior"));
	static ConstructorHelpers::FClassFinder<AUdemyMultiplayerCharacter> ArcherBPClass(TEXT("/Game/Blueprints/Characters/BP_Archer"));
	static ConstructorHelpers::FClassFinder<AUdemyMultiplayerCharacter> WizardBPClass(TEXT("/Game/Blueprints/Characters/BP_Wizard"));

	if (!ensure(WarriorBPClass.Class != nullptr)) return;
	if (!ensure(ArcherBPClass.Class != nullptr)) return;
	if (!ensure(WizardBPClass.Class != nullptr)) return;

	this->Characters.Add(WarriorBPClass.Class);
	this->Characters.Add(ArcherBPClass.Class);
	this->Characters.Add(WizardBPClass.Class);

	DefaultCharacterChampion = this->Characters[0];
	
	PlayerControllerClass = ALobbyPlayerController::StaticClass();	

	UWorld* World = GetWorld();

    if (IsValid(World))
    {
        UGameInstance* GameInstance = World->GetGameInstance();

        if (IsValid(GameInstance))
            UdemyMultiplayerGameInstance = Cast<UUdemyMultiplayerGameInstance>(GameInstance);
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

                UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALobbyPlayerSpot::StaticClass(), this->SpawnPoints);

                FConfigurationMaps* DefaultMap = UdemyMultiplayerGameInstance->ConfigurationMaps.Find(TEXT("LobbyMap"));

                UTexture2D* mapImage = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *DefaultMap->ImagePath));
                
				LobbyPlayerController->Client_SetupLobbyMenu(UdemyMultiplayerGameInstance->ServerName);

				LobbyPlayerController->Client_UpdateLobbySettings(mapImage, *DefaultMap->Name);

				Server_SpawnLobbyPlayerSpot(LobbyPlayerController);

                Server_RespawnPlayer(LobbyPlayerController);
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

void ALobbyGameMode::Server_SwapCharacter_Implementation(APlayerController* PlayerController, int32 playerCharacterIndex, bool bChangeStatus)
{
    if (!bChangeStatus) {

        if (this->Characters.IsValidIndex(playerCharacterIndex)) {
			ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(PlayerController);
            this->DefaultCharacterChampion = this->Characters[playerCharacterIndex];
            this->ShowCharacterSelectedInLobby(LobbyPlayerController);
        }
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

FPlayerSpot* ALobbyGameMode::GetPlayerSpotForPlayerConnected()
{
	FPlayerSpot* PlayerSpotIndex = UdemyMultiplayerGameInstance->ConfigurationPlayerSpot.Find(this->AllPlayerControllers.Num());
	
	return PlayerSpotIndex;
}

void ALobbyGameMode::Server_SpawnLobbyPlayerSpot_Implementation(ALobbyPlayerController* LobbyPlayerController)
{
    FPlayerSpot* PlayerSpotByIndex = GetPlayerSpotForPlayerConnected();

    if (PlayerSpotByIndex != nullptr)
    {
        FActorSpawnParameters params;

        params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        ALobbyPlayerSpot* LobbyPlayerSpotSpawned = Cast<ALobbyPlayerSpot>(GetWorld()->SpawnActor<ALobbyPlayerSpot>(LobbyPlayerSpotClass, PlayerSpotByIndex->Location, PlayerSpotByIndex->Rotation, params));

        LobbyPlayerController->SetPlayerSpot(LobbyPlayerSpotSpawned);
    }
}

void ALobbyGameMode::Server_RespawnPlayer_Implementation(ALobbyPlayerController* LobbyPlayerController)
{
	this->ShowCharacterSelectedInLobby(LobbyPlayerController);
}

void ALobbyGameMode::Server_EveryoneUpdate_Implementation() 
{
	this->CurrentPlayers = this->AllPlayerControllers.Num();

	if (this->CurrentPlayers > 0)	
		this->ConnectedPlayers.Empty();

	for (ALobbyPlayerController* PlayerController : this->AllPlayerControllers)
	{
		this->ConnectedPlayers.Add(PlayerController->PlayerSettings);	
	}

	for (ALobbyPlayerController* PlayerController : this->AllPlayerControllers)
	{
		PlayerController->Client_UpdateNumberOfPlayers(this->CurrentPlayers, this->MaxPlayers);

		PlayerController->Client_AddPlayersInfo(this->ConnectedPlayers);
	}
}

void ALobbyGameMode::LaunchTheGame() 
{
	UWorld* World = GetWorld();

	const FString FullMapPath = FString(TEXT("/Game/ThirdPerson/Maps/{0}?listen: {0}"));

	if (World)
		World->ServerTravel(FString::Format(*FullMapPath, { MapName }));
}

ALobbyPlayerSpot* ALobbyGameMode::GetFreePlayerSpot() 
{
	for (AActor* ActorSpot : this->SpawnPoints)
	{
		ALobbyPlayerSpot* LobbyPlayerSpot = Cast<ALobbyPlayerSpot>(ActorSpot);

		if (!LobbyPlayerSpot->GetIsUsed())
			return LobbyPlayerSpot;
	}

	return nullptr;
}

void ALobbyGameMode::SpawnCharacterOnPlayerSpot(ALobbyPlayerController* LobbyPlayerController)
{
	FActorSpawnParameters params;

	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ALobbyPlayerSpot* LobbyPlayerSpot = LobbyPlayerController->GetPlayerSpot();

	AUdemyMultiplayerCharacter* SpawnCharacter = Cast<AUdemyMultiplayerCharacter>(GetWorld()->SpawnActor<AUdemyMultiplayerCharacter>(DefaultCharacterChampion, LobbyPlayerSpot->GetActorTransform(), params));

	FTransform ActorTransform = LobbyPlayerSpot->GetActorTransform();
	
	FVector	Location = ActorTransform.GetLocation();

	Location[2] = 90;

	ActorTransform.SetLocation(Location);

	SpawnCharacter->SetActorTransform(ActorTransform);

	LobbyPlayerSpot->SetIsUsed(true);

	LobbyPlayerController->SetCurrentCharacter(SpawnCharacter);
}

void ALobbyGameMode::ShowCharacterSelectedInLobby(ALobbyPlayerController* LobbyPlayerController)
{
    AUdemyMultiplayerCharacter* UdemyMultiplayerCharacter = LobbyPlayerController->GetCurrentCharacter();
    ALobbyPlayerSpot* LobbyPlayerSpot = LobbyPlayerController->GetPlayerSpot();

    if (UdemyMultiplayerCharacter != nullptr)
        UdemyMultiplayerCharacter->Destroy();

    if (IsValid(LobbyPlayerSpot))
        this->SpawnCharacterOnPlayerSpot(LobbyPlayerController);

    Server_EveryoneUpdate();
}

void ALobbyGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGameMode, AllPlayerControllers);
	DOREPLIFETIME(ALobbyGameMode, SpawnPoints);
	DOREPLIFETIME(ALobbyGameMode, MapImage);
	DOREPLIFETIME(ALobbyGameMode, MapName);
	DOREPLIFETIME(ALobbyGameMode, CurrentPlayers);
	DOREPLIFETIME(ALobbyGameMode, MaxPlayers);
	DOREPLIFETIME(ALobbyGameMode, ConnectedPlayers);
	DOREPLIFETIME(ALobbyGameMode, Characters);
}