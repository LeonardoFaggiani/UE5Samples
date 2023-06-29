// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "GameMode/LobbyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UdemyMultiplayerPlayerState.h"
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

	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this->GetWorld());

	if(IsValid(GameMode))
		LobbyGameMode = Cast<ALobbyGameMode>(GameMode);
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

	Lobby = CreateWidget<ULobby>(this, LobbyClass);
	CharacterSelection = CreateWidget<UCharacterSelection>(this, CharacterSelectionClass);

	if (!ensure(Lobby != nullptr)) return;
	if (!ensure(CharacterSelection != nullptr)) return;

	this->CharacterSelection->AddToViewport();

	Lobby->CharacterSelectionContainer->AddChild(CharacterSelection);
	Lobby->SetServerName(ServerName);
	Lobby->AddToViewport();

	this->bShowMouseCursor = true;
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
	Lobby->TearDown();

	ULoadingScreen* LoadingScreen = CreateWidget<ULoadingScreen>(this, ULoadingScreen::StaticClass());

	if (!ensure(LoadingScreen != nullptr)) return;

	LoadingScreen->Setup();
}

void ALobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerController, PlayerSettings);
	DOREPLIFETIME(ALobbyPlayerController, PlayerSpot);
}