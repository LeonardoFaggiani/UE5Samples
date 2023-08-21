// Fill out your copyright notice in the Description page of Project Settings.


#include "OverheadPlayerSpot.h"

void UOverheadPlayerSpot::SetPlayerName(FString PlayerName)
{
    this->LobbyPlayerName->SetText(FText::FromString(PlayerName));
}

void UOverheadPlayerSpot::SetReadyStatus(bool bIsReady)
{
    UTexture2D* ReadyStatusIconTexture = bIsReady ? 
        Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Game/UI/Materials/Textures/checkmark-small"))) 
        : 
        Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Game/UI/Materials/Textures/cancel-small")));

    if (IsValid(ReadyStatusIconTexture))
        this->ReadyStatusIcon->SetBrushFromTexture(ReadyStatusIconTexture);

    FString color = bIsReady ? FString("#38E33F") : FString("#FF3122");

    this->SetPlayerNameColor(color);
}

void UOverheadPlayerSpot::SetReadyStatusVisibility(bool bIsHidden)
{
    ESlateVisibility SlateVisibility = bIsHidden ? ESlateVisibility::Collapsed : ESlateVisibility::Visible;
    
    this->ReadyStatusIcon->SetVisibility(SlateVisibility);
}

void UOverheadPlayerSpot::SetPlayerNameColor(FString Color)
{
    FColor HexColor = FColor::FromHex(Color);

    FLinearColor LinearHexColor(HexColor);

    this->LobbyPlayerName->SetColorAndOpacity(FSlateColor(LinearHexColor));
}

void UOverheadPlayerSpot::NativeDestruct()
{
    RemoveFromParent();

    Super::NativeDestruct();
}