// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerSpot.generated.h"

USTRUCT(BlueprintType)
struct FPlayerSpot
{

    GENERATED_BODY()

public:
    FPlayerSpot();
    FPlayerSpot(int32 Index, FVector Location, FRotator Rotation);
    ~FPlayerSpot();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby Player Spot")
        int32 Index;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby Player Spot")
        FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby Player Spot")
        FRotator Rotation;
};