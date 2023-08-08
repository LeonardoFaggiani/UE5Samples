// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "GameMode/LobbyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputTriggers.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ALobbyPlayerController::ALobbyPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> LobbyBPClass(TEXT("/Game/UI/Menu/WBP_Lobby"));
	static ConstructorHelpers::FClassFinder<UUserWidget> CharacterSelectionBPClass(TEXT("/Game/UI/Menu/WBP_CharacterSelection"));

	if (!ensure(LobbyBPClass.Class != nullptr)) return;
	if (!ensure(CharacterSelectionBPClass.Class != nullptr)) return;

	LobbyClass = LobbyBPClass.Class;
	CharacterSelectionClass = CharacterSelectionBPClass.Class;

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

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(LobbyPlayerControllerMappingContext, 0);
	}
}

void ALobbyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {

        //Open character selection menu
        EnhancedInputComponent->BindAction(ToggleMenuAction, ETriggerEvent::Triggered, this, &ALobbyPlayerController::ToggleCharacterSelectionMenu);
    }
}

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

			if (actor->GetActorLabel() == InActorName)
				return actor;
		}
	}

	return nullptr;
}

void ALobbyPlayerController::ToggleCharacterSelectionMenu(const FInputActionValue& Value)
{
	if (IsValid(this->CharacterSelection))
	{
		this->CharacterSelection->ToggleMenu();
	}
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
	ALobbyPlayerSpot* LobbyPlayerSpot = this->GetPlayerSpot();

	if (IsValid(LobbyPlayerSpot)) {
		LobbyPlayerSpot->SetIsReady(this->PlayerSettings.bPlayerReadyState);
		LobbyPlayerSpot->OnRep_ReadyStateUpdated();
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

void ALobbyPlayerController::Client_SetupLobbyMenu_Implementation(const FString& ServerName)
{
	if (!ensure(LobbyClass != nullptr)) return;
	if (!ensure(CharacterSelectionClass != nullptr)) return;

	this->Lobby = CreateWidget<ULobby>(this, LobbyClass);
	this->CharacterSelection = CreateWidget<UCharacterSelection>(this, CharacterSelectionClass);

	if (!ensure(this->Lobby != nullptr)) return;
	if (!ensure(this->CharacterSelection != nullptr)) return;

	this->CharacterSelection->Setup();

	this->Lobby->CharacterSelectionContainer->AddChild(CharacterSelection);
	this->Lobby->SetServerName(ServerName);
	this->Lobby->Setup();
}

void ALobbyPlayerController::Server_CallUpdate_Implementation(const FLobbyPlayerInfo& PlayerInfo)
{
	PlayerSettings = PlayerInfo;

	if (IsValid(LobbyGameMode)) {
		LobbyGameMode->Server_SwapCharacter(this, PlayerSettings.PlayerCharacterIndex, PlayerSettings.bPlayerReadyState);
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

void ALobbyPlayerController::Client_UpdateLobbySettings_Implementation(UTexture2D* MapImage, const FString& MapName)
{
	if (!IsValid(Lobby)) return;

	Lobby->SetMap(MapImage, MapName);
}

void ALobbyPlayerController::Client_UpdateNumberOfPlayers_Implementation(int32 CurrentPlayers, int32 MaxPlayers)
{
	if (!IsValid(Lobby)) return;

	TArray<FStringFormatArg> CurrentAndMaxPlayers;
	CurrentAndMaxPlayers.Add(FStringFormatArg(CurrentPlayers));
	CurrentAndMaxPlayers.Add(FStringFormatArg(MaxPlayers));

	FString CurrentAndMaxPlayersFormat = FString::Format(TEXT("{0}/{1} players"), CurrentAndMaxPlayers);

	if (IsValid(LobbyGameMode))
		Lobby->SetEnablePlayButton(LobbyGameMode->IsAllPlayerReady());
	
	Lobby->SetCurrentPlayersFormat(CurrentAndMaxPlayersFormat);
}

void ALobbyPlayerController::Client_AssignPlayer_Implementation(int32 CharacterSelected)
{
	if (this->PlayerSettings.bPlayerReadyState)
		return;

	this->PlayerSettings.PlayerCharacterIndex = CharacterSelected;	

	this->Server_CallUpdate(this->PlayerSettings);
}

void ALobbyPlayerController::Client_ShowLoadingScreen_Implementation() 
{
	UdemyMultiplayerGameInstance->ShowLoadingScreen(true);
}

void ALobbyPlayerController::Client_SetViewTargetSpot_Implementation()
{
	this->ViewTarget = GetActorByName("ViewTargetSpot");

	this->SetViewTargetWithBlend(this->ViewTarget);
}

void ALobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerController, PlayerSettings);
	DOREPLIFETIME(ALobbyPlayerController, PlayerSpot);
}