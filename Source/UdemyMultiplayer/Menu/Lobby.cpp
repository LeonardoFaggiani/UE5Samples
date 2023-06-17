// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby.h"
#include "Net/UnrealNetwork.h"
#include "../GameMode/LobbyGameMode.h"
#include "../LobbyPlayerController.h"
#include "Kismet/GameplayStatics.h"

bool ULobby::Initialize()
{
    if (!Super::Initialize())
        return false;

    UGameInstance* GameInstance = GetGameInstance();

    if (GameInstance) {
        UdemyMultiplayerGameInstance = Cast<UUdemyMultiplayerGameInstance>(GameInstance);
        
        AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GameInstance->GetWorld());

        ALobbyGameMode* lobbyGameMode = Cast<ALobbyGameMode>(GameMode);

        if (lobbyGameMode != nullptr) {
            this->LobbyGameMode = lobbyGameMode;
        }
    }

    if (ReadyButton && PlayButton && PreviousMap && NextMap) {

        ReadyButton->OnClicked.AddDynamic(this, &ULobby::OnReadyButtonClicked);
        PlayButton->OnClicked.AddDynamic(this, &ULobby::OnPlayButtonClicked);

        PreviousMap->OnClicked.AddDynamic(this, &ULobby::OnPreviousMapButtonClicked);
        NextMap->OnClicked.AddDynamic(this, &ULobby::OnNextMapButtonClicked);


        this->ShowOrHideButton();
    }

    //this->CharacterSelectionContainer->SetVisibility(ESlateVisibility::Hidden);

    this->InitializeMap();

    return true;
}

void ULobby::ClearPlayerList() {
    PlayerListInLobby->ClearChildren();
}

void ULobby::SetServerName(FString serverName) {
    this->ServerName->SetText(FText::FromString(serverName));
}

void ULobby::ShowOrHideButton() {

    UWorld* World = GetWorld();

    if (World == nullptr)
        return;

    if (ReadyButton && PlayButton && PreviousMap && NextMap) {

        if (World->IsServer()) {
            this->PlayButton->SetVisibility(ESlateVisibility::Visible);
            this->ReadyButton->SetVisibility(ESlateVisibility::Hidden);
        }
        else {
            this->PlayButton->SetVisibility(ESlateVisibility::Hidden);
            this->ReadyButton->SetVisibility(ESlateVisibility::Visible);
        }
    }
}

#pragma region

void ULobby::OnReadyButtonClicked()
{
    UpdateStatus();
}

void ULobby::OnPlayButtonClicked()
{
    for (ALobbyPlayerController* PlayerController : this->LobbyGameMode->AllPlayerControllers)
    {
        PlayerController->Client_ShowLoadingScreen();
    }

    this->LobbyGameMode->LaunchTheGame();
}

void ULobby::UpdateStatus() 
{
    
}

void ULobby::OnPreviousMapButtonClicked()
{
    FConfigurationMaps* FConfigurationMaps = this->GetPreviousNextMap(false);

    UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *FConfigurationMaps->ImagePath));

    this->SetMap(Texture, *FConfigurationMaps->Name);

    this->NotifyMapChaged();
}

void ULobby::OnNextMapButtonClicked()
{
    FConfigurationMaps* FConfigurationMaps = this->GetPreviousNextMap(true);

    UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *FConfigurationMaps->ImagePath));

    this->SetMap(Texture, *FConfigurationMaps->Name);

    this->NotifyMapChaged();
}

#pragma endregion Buttons

#pragma region

void ULobby::InitializeMap()
{
    FConfigurationMaps* FConfigurationMaps = this->GetCurrentMapByName(TEXT("LobbyMap"));

    if (FConfigurationMaps != nullptr)
    {
        UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *FConfigurationMaps->ImagePath));

        this->SetMap(Texture, *FConfigurationMaps->Name);
    }
}

FConfigurationMaps* ULobby::GetCurrentMapByName(FString Name)
{
    if (UdemyMultiplayerGameInstance != nullptr)
    {
        FConfigurationMaps* DefaultMap = UdemyMultiplayerGameInstance->ConfigurationMaps.Find(Name);

        return DefaultMap;
    }

    return nullptr;
}

FConfigurationMaps* ULobby::GetPreviousNextMap(bool IsIncrement)
{
    FName CurrentMapName = MapImage->Brush.GetResourceName();

    FConfigurationMaps* CurrentMap = this->GetCurrentMapByName(CurrentMapName.ToString());

    if (CurrentMap == nullptr)
        return nullptr;

    FConfigurationMaps* PreviousOrNextMap = nullptr;

    int PreviousNextOrder = IsIncrement ? (CurrentMap->Order + 1) : (CurrentMap->Order - 1);

    for (TPair<FString, FConfigurationMaps> Map : UdemyMultiplayerGameInstance->ConfigurationMaps)
    {
        if (Map.Value.Order == PreviousNextOrder)
        {
            PreviousOrNextMap = this->GetCurrentMapByName(Map.Key);
        }
    }

    if (PreviousOrNextMap == nullptr)
    {
        PreviousOrNextMap = IsIncrement ? this->GetCurrentMapByName(TEXT("LobbyMap")) : this->GetCurrentMapByName(TEXT("EgyptMap"));
    }

    return PreviousOrNextMap;
}

void ULobby::NotifyMapChaged() {
    this->LobbyGameMode->Server_UpdateGameSettings(this->CurrentTextureMap, this->MapName->GetText().ToString());
}

#pragma endregion Map

void ULobby::SetCurrentPlayersFormat(FString currentPlayersFormat) {
    this->CurrentPlayersFormat->SetText(FText::FromString(currentPlayersFormat));
}

void ULobby::Client_UpdatePlayerList_Implementation(const FLobbyPlayerInfo& PlayerInfo) {
    
    UPlayerLobbyList* PlayerLobbyList = CreateWidget<UPlayerLobbyList>(this, PlayerLobbyListClass);
    
    PlayerLobbyList->SetPlayerName(PlayerInfo.PlayerName);
    PlayerLobbyList->SetIsReady(PlayerInfo.bPlayerReadyState);
    
    this->PlayerListInLobby->AddChild(PlayerLobbyList);    
}

void ULobby::SetMap(UTexture2D* mapImage, FString mapName) {

    this->MapImage->SetVisibility(ESlateVisibility::Visible);
    this->MapImage->SetBrushFromTexture(mapImage);

    this->MapName->SetText(FText::FromString(mapName));

    this->CurrentTextureMap = mapImage;
}

void ULobby::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ULobby, MapImage);
    DOREPLIFETIME(ULobby, MapName);
}