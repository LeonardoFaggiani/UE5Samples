// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "../UdemyMultiplayerGameInstance.h"

bool UMainMenu::Initialize()
{
    if (!Super::Initialize())
        return false;

    if (ExitButton && HostButton && FindGamesButton) {
        ExitButton->OnClicked.AddDynamic(this, &ThisClass::OnExitButtonClick);
        HostButton->OnClicked.AddDynamic(this, &ThisClass::OnHostButtonClicked);
        FindGamesButton->OnClicked.AddDynamic(this, &ThisClass::OnFindGamesButtonClick);
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

    this->UdemyMultiplayerGameInstance->OpenNextLevel(FName("MainMenuWidgets"), false, false, 0.1f);

}

void UMainMenu::OnFindGamesButtonClick()
{
    this->UdemyMultiplayerGameInstance->SetHostGame(false);
    this->UdemyMultiplayerGameInstance->SetFindGames(true);

    this->UdemyMultiplayerGameInstance->OpenNextLevel(FName("MainMenuWidgets"), false, false, 0.1f);
}

void UMainMenu::OnExitButtonClick() {
    if (MenuInterface != nullptr) MenuInterface->Quit();
}