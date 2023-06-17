// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerLobbyList.h"
#include "Net/UnrealNetwork.h"

void UPlayerLobbyList::SetPlayerName(FString playerName)
{
    PlayerName = playerName;
    PlayerNameText->SetText(FText::FromString(PlayerName));
}

void UPlayerLobbyList::SetIsReady(bool bIsReady)
{
    PlayerStatus = bIsReady ? "Ready" : "Unready";
    
    PlayerStatusText->SetText(FText::FromString(PlayerStatus));
}

void UPlayerLobbyList::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UPlayerLobbyList, PlayerStatus);
    DOREPLIFETIME(UPlayerLobbyList, PlayerName);
}