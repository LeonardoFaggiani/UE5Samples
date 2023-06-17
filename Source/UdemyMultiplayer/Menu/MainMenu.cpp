// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"

bool UMainMenu::Initialize()
{
    if (!Super::Initialize())
        return false;

    if (HostButton) {
        HostButton->OnClicked.AddDynamic(this, &ThisClass::OnHostButtonClicked);
    }

    if (FindGamesButton) {
        FindGamesButton->OnClicked.AddDynamic(this, &ThisClass::OnFindGamesButtonClick);
    }

    if (ExitButton) {
        ExitButton->OnClicked.AddDynamic(this, &ThisClass::OnExitButtonClick);
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
    HostButton->SetIsEnabled(false);
    
    if (MenuInterface != nullptr) MenuInterface->Host();
}

void UMainMenu::OnFindGamesButtonClick()
{
    FindGamesButton->SetIsEnabled(false);

    if (MenuInterface != nullptr) MenuInterface->Join(false);
}

void UMainMenu::OnExitButtonClick() {
    if (MenuInterface != nullptr) MenuInterface->Quit();
}

UWidgetSwitcher* UMainMenu::GetMenuSwitcher() {
    return this->WidgetSwitcherMenu;
}
