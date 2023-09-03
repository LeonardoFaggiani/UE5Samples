// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "../UdemyMultiplayerGameInstance.h"

bool UMainMenu::Initialize()
{
    if (!Super::Initialize())
        return false;

    if (ExitButton && HostButton && FindGamesButton) {
        HostButton->OnClicked().AddUObject(this, &ThisClass::OnHostButtonClicked);
        FindGamesButton->OnClicked().AddUObject(this, &ThisClass::OnFindGamesButtonClicked);
        OptionsButton->OnClicked().AddUObject(this, &ThisClass::OnOptionsButtonButtonClicked);
        ExitButton->OnClicked().AddUObject(this, &ThisClass::OnExitButtonClicked);
    }

    UWorld* World = GetWorld();

    if (IsValid(World))
    {
        UGameInstance* GameInstance = World->GetGameInstance();

        if (IsValid(GameInstance))
            UdemyMultiplayerGameInstance = Cast<UUdemyMultiplayerGameInstance>(GameInstance);        
    }


    return true;
}

void UMainMenu::NativeDestruct()
{
    TearDown();

    Super::NativeDestruct();
}

void UMainMenu::OnHostButtonClicked()
{
    this->UdemyMultiplayerGameInstance->SetHostGame(true);
    this->UdemyMultiplayerGameInstance->SetFindGames(false);
    this->UdemyMultiplayerGameInstance->SetOptionsMenu(false);

    this->UdemyMultiplayerGameInstance->OpenNextLevel(FName("MainMenuWidgets"), false, false, 0.1f);
}

void UMainMenu::OnFindGamesButtonClicked()
{
    this->UdemyMultiplayerGameInstance->SetFindGames(true);
    this->UdemyMultiplayerGameInstance->SetHostGame(false);
    this->UdemyMultiplayerGameInstance->SetOptionsMenu(false);

    this->UdemyMultiplayerGameInstance->OpenNextLevel(FName("MainMenuWidgets"), false, false, 0.1f);
}

void UMainMenu::OnOptionsButtonButtonClicked()
{
    this->UdemyMultiplayerGameInstance->SetOptionsMenu(true);
    this->UdemyMultiplayerGameInstance->SetHostGame(false);
    this->UdemyMultiplayerGameInstance->SetFindGames(false);

    this->UdemyMultiplayerGameInstance->OpenNextLevel(FName("MainMenuWidgets"), false, false, 0.1f);
}

void UMainMenu::OnExitButtonClicked() {
    if (MenuInterface != nullptr) MenuInterface->Quit();
}