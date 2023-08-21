// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Enum/AttributeType.h"
#include "HeroeAttribute.h"
#include "UdemyMultiplayer/UdemyMultiplayerCharacter.h"
#include "HeroeResources.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FHeroeResources
{
	GENERATED_BODY()

public:
    FHeroeResources(FString InName, TSubclassOf<AUdemyMultiplayerCharacter> InTargetClass, FString InHeroeIcon, TArray<struct FHeroeAttribute>  InHeroeAttribute);
    FHeroeResources();
	~FHeroeResources();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heroe Attributes")
        FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heroe Attributes")
        TSubclassOf<AUdemyMultiplayerCharacter> TargetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heroe Attributes")
        FString HeroeIcon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heroe Attributes")
        TArray<struct FHeroeAttribute>  HeroeAttributes;
};
