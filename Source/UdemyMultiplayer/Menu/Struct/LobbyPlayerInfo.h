// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../UdemyMultiplayerCharacter.h"
#include "LobbyPlayerInfo.generated.h"

USTRUCT(BlueprintType)
struct FLobbyPlayerInfo
{

    GENERATED_BODY()

public:
    FLobbyPlayerInfo();
    ~FLobbyPlayerInfo();
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby Player Info")
        bool bPlayerReadyState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby Player Info")
        FString PlayerName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby Player Info")
        TSubclassOf<AUdemyMultiplayerCharacter> HeroeSelected;
};