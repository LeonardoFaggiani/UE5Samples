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

            this->InitializeMap();
        }
    }

    if (ReadyButton && ReadyUpButton && PreviousMap && NextMap && HeroesButton) {

        ReadyUpButton->OnClicked().AddUObject(this, &ThisClass::OnPlayButtonClicked);
        ReadyButton->OnClicked().AddUObject(this, &ThisClass::OnReadyButtonClicked);
        HeroesButton->OnClicked().AddUObject(this, &ThisClass::OnHeroesButtonClicked);

        PreviousMap->OnClicked.AddDynamic(this, &ULobby::OnPreviousMapButtonClicked);
        NextMap->OnClicked.AddDynamic(this, &ULobby::OnNextMapButtonClicked);
        
        this->ShowOrHideButton();
    }

    return true;
}

void ULobby::SetServerName(FString serverName) {
    this->ServerName->SetText(FText::FromString(serverName));
}

void ULobby::ShowOrHideButton() {

    UWorld* World = GetWorld();

    if (World == nullptr)
        return;

    if (ReadyUpButton && ReadyButton && PreviousMap && NextMap) {

        if (!GetWorld()->IsServer()) {
            this->ReadyUpButton->SetVisibility(ESlateVisibility::Hidden);
            this->ReadyButton->SetVisibility(ESlateVisibility::Visible);

            this->PreviousMap->SetVisibility(ESlateVisibility::Collapsed);
            this->NextMap->SetVisibility(ESlateVisibility::Collapsed);
        }

        this->SetEnablePlayButton(false);
    }
}

#pragma region

void ULobby::OnPlayButtonClicked()
{
    if (this->LobbyGameMode->IsAllPlayerReady())
    {
        for (ALobbyPlayerController* PlayerController : this->LobbyGameMode->AllPlayerControllers)        
            PlayerController->Client_ShowLoadingScreen();           

        this->LobbyGameMode->LaunchTheGame();
    }    
}

void ULobby::OnHeroesButtonClicked()
{
    UHeroeSelection* InHeroeSelection = CreateWidget<UHeroeSelection>(this, this->HeroeSelection);

    InHeroeSelection->Setup();

    this->ReadyButton->SetIsEnabled(false);

    this->SetHiddenHeroesButton(true);
}

void ULobby::OnReadyButtonClicked()
{
    UpdateStatus();
}

void ULobby::UpdateStatus() 
{    
    ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(this->GetOwningPlayer());
    
    if (IsValid(LobbyPlayerController))
    {
        LobbyPlayerController->PlayerSettings.bPlayerReadyState = !LobbyPlayerController->PlayerSettings.bPlayerReadyState;

        LobbyPlayerController->UpdateReadyState();

        LobbyPlayerController->Server_NotifyPlayerStatus(LobbyPlayerController->PlayerSettings);     
        
        this->HeroesButton->SetIsEnabled(!LobbyPlayerController->PlayerSettings.bPlayerReadyState);        
    }
}

void ULobby::SetHiddenHeroesButton(bool bHidden) {
    this->HeroesButton->SetVisibility(bHidden ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
}


void ULobby::SetEnablePlayButton(bool bEnabled) {
    this->ReadyUpButton->SetIsEnabled(bEnabled);
}

void ULobby::OnPreviousMapButtonClicked()
{
    FConfigurationMaps* ConfigurationMaps = this->GetPreviousNextMap(false);
    
    this->SetMapSelector(ConfigurationMaps);
}

void ULobby::OnNextMapButtonClicked()
{
    FConfigurationMaps* ConfigurationMaps = this->GetPreviousNextMap(true);

    this->SetMapSelector(ConfigurationMaps);
}

#pragma endregion Buttons

#pragma region

void ULobby::InitializeMap()
{
    FConfigurationMaps FConfigurationMaps = GetFirstOrLastMap(false);
    
    UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *FConfigurationMaps.ImagePath));
    
    this->SetMap(Texture, FConfigurationMaps.Name);
    
    if (GetWorld()->IsServer())
        this->NotifyMapChaged();    
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

FConfigurationMaps ULobby::GetFirstOrLastMap(bool bIsFirst)
{
    TArray<FConfigurationMaps> ConfigurationDataArray;
    
    UdemyMultiplayerGameInstance->ConfigurationMaps.GenerateValueArray(ConfigurationDataArray);

    ConfigurationDataArray.Sort([bIsFirst](const FConfigurationMaps A, const FConfigurationMaps B) {
        return bIsFirst ? A.Order < B.Order : A.Order > B.Order;
    });

    return ConfigurationDataArray[0];
}

FConfigurationMaps* ULobby::GetPreviousNextMap(bool IsIncrement)
{
    PreviousMap->SetIsEnabled(true);
    NextMap->SetIsEnabled(true);

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

    int VerifyIfExistsPreviousOrNextMap = IsIncrement ? (PreviousNextOrder + 1) : (PreviousNextOrder - 1);

    FConfigurationMaps* ExistsPreviousOrNextMap = nullptr;

    for (TPair<FString, FConfigurationMaps> Map : UdemyMultiplayerGameInstance->ConfigurationMaps)
    {
        if (Map.Value.Order == VerifyIfExistsPreviousOrNextMap)
        {
            ExistsPreviousOrNextMap = this->GetCurrentMapByName(Map.Key);
        }
    }

    if (ExistsPreviousOrNextMap == nullptr) {
        if (IsIncrement) {
            PreviousMap->SetIsEnabled(true);
            NextMap->SetIsEnabled(false);
        }
        else {
            PreviousMap->SetIsEnabled(false);
            NextMap->SetIsEnabled(true);
        }
    }

    return PreviousOrNextMap;
}

void ULobby::SetMapSelector(FConfigurationMaps* ConfigurationMaps)
{
    if (ConfigurationMaps != nullptr)
    {
        UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *ConfigurationMaps->ImagePath));

        this->SetMap(Texture, *ConfigurationMaps->Name);

        this->NotifyMapChaged();
    }
}

void ULobby::NotifyMapChaged() {
    this->LobbyGameMode->Server_UpdateGameSettings(this->CurrentTextureMap, this->MapName->GetText().ToString());
}

#pragma endregion Map

void ULobby::SetCurrentPlayersFormat(FString currentPlayersFormat) {
    this->CurrentPlayersFormat->SetText(FText::FromString(currentPlayersFormat));
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