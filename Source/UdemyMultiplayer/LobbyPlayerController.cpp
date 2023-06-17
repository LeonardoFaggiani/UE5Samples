// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "GameMode/LobbyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UdemyMultiplayerPlayerState.h"
#include "EnhancedInputSubsystems.h"

ALobbyPlayerController::ALobbyPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> LobbyBPClass(TEXT("/Game/UI/Menu/WBP_Lobby"));

	if (!ensure(LobbyBPClass.Class != nullptr)) return;

	LobbyClass = LobbyBPClass.Class;	

	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this->GetWorld());

	if(IsValid(GameMode))
		LobbyGameMode = Cast<ALobbyGameMode>(GameMode);
}

void ALobbyPlayerController::SetCurrentCharacter(AUdemyMultiplayerCharacter* currentCharacter)
{
	this->CurrentCharacter = currentCharacter;
}

AUdemyMultiplayerCharacter* ALobbyPlayerController::GetCurrentCharacter()
{
	return this->CurrentCharacter;
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
	Lobby = CreateWidget<ULobby>(this, LobbyClass);

	if (!ensure(Lobby != nullptr)) return;

	Lobby->SetServerName(ServerName);
	Lobby->Setup();
}

void ALobbyPlayerController::Server_CallUpdate_Implementation(const FLobbyPlayerInfo& PlayerInfo)
{
	PlayerSettings = PlayerInfo;

	if (LobbyGameMode != nullptr) {
		LobbyGameMode->Server_SwapCharacter(this, PlayerSettings.PlayerCharacterIndex, PlayerSettings.bPlayerReadyState);
		LobbyGameMode->Server_EveryoneUpdate();
	}
}

void ALobbyPlayerController::Client_AddPlayersInfo_Implementation(const TArray<struct FLobbyPlayerInfo>& ConnectedPlayersInfo)
{
	this->AllConnectedPlayers = ConnectedPlayersInfo;

	if (IsValid(Lobby)) {

		Lobby->ClearPlayerList();

		for (FLobbyPlayerInfo PlayerInfo : this->AllConnectedPlayers)
			Lobby->Client_UpdatePlayerList(PlayerInfo);
	}
}

void ALobbyPlayerController::Client_UpdateLobbySettings_Implementation(UTexture2D* MapImage, const FString& MapName)
{
	if (Lobby == nullptr) return;

	Lobby->SetMap(MapImage, MapName);
}

void ALobbyPlayerController::Client_UpdateNumberOfPlayers_Implementation(int32 CurrentPlayers, int32 MaxPlayers)
{
	if (Lobby == nullptr) return;

	TArray<FStringFormatArg> CurrentAndMaxPlayers;
	CurrentAndMaxPlayers.Add(FStringFormatArg(CurrentPlayers));
	CurrentAndMaxPlayers.Add(FStringFormatArg(MaxPlayers));

	FString CurrentAndMaxPlayersFormat = FString::Format(TEXT("{0}/{1} players"), CurrentAndMaxPlayers);

	Lobby->SetCurrentPlayersFormat(CurrentAndMaxPlayersFormat);
}

void ALobbyPlayerController::Client_AssignPlayer_Implementation(int32 CharacterSelected)
{
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
	DOREPLIFETIME(ALobbyPlayerController, AllConnectedPlayers);	
}