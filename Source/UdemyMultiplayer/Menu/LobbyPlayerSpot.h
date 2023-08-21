// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LobbyPlayerSpot.generated.h"

UCLASS()
class UDEMYMULTIPLAYER_API ALobbyPlayerSpot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALobbyPlayerSpot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
