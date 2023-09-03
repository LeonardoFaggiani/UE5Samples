// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Menu/HeroeItem.h"
#include "UdemyMultiplayerPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYMULTIPLAYER_API AUdemyMultiplayerPlayerState : public APlayerState
{
	GENERATED_BODY()

	AUdemyMultiplayerPlayerState();

public:
	void AddHeroeItem(UHeroeItem* HeroeItem);
	TArray<UHeroeItem*> GetHeroesItems();

private:
	TArray<UHeroeItem*> HeroesItems;
};
