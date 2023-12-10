// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"

bool UInGameMenu::Initialize()
{
    if (!Super::Initialize())
        return false;

    if (OptionsButton) {
        OptionsButton->OnClicked().AddUObject(this, &UInGameMenu::OnOptionsButtonClicked);
    }

    return true;
}

void UInGameMenu::NativeDestruct()
{
    TearDown();

    Super::NativeDestruct();
}

void UInGameMenu::OnOptionsButtonClicked()
{
    if (!ensure(this->OptionsMenuClass != nullptr)) return;

    this->OptionsMenu = CreateWidget<UOptionsMenu>(this, OptionsMenuClass);
    this->OptionsMenu->Setup();
}