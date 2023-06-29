// Fill out your copyright notice in the Description page of Project Settings.


#include "OverheadPlayerSpot.h"

void UOverheadPlayerSpot::SetPlayerName(FString PlayerName)
{
    this->LobbyPlayerName->SetText(FText::FromString(PlayerName));
}

void UOverheadPlayerSpot::NativeDestruct()
{
    RemoveFromParent();

    Super::NativeDestruct();
}