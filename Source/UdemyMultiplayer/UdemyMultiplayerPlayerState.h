// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "UdemyMultiplayerPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYMULTIPLAYER_API AUdemyMultiplayerPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AUdemyMultiplayerPlayerState();

	UPROPERTY(Replicated)
		bool bIsReady;
};
