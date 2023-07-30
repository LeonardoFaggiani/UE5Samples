// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Engine/StaticMesh.h"
#include "OverheadPlayerSpot.h"
#include "Components/WidgetComponent.h"
#include "LobbyPlayerSpot.generated.h"

UCLASS()
class UDEMYMULTIPLAYER_API ALobbyPlayerSpot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALobbyPlayerSpot();

	void SetIsReady(bool bReady);

	UFUNCTION()
	void OnRep_ReadyStateUpdated();

	UPROPERTY(ReplicatedUsing = OnRep_ReadyStateUpdated)
	bool bReady;

	UFUNCTION(NetMulticast, Reliable)
	void Multi_SetPlayerName(const FString& InPlayerName);
	void Multi_SetPlayerName_Implementation(const FString& InPlayerName);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnyWhere, Category = "Materials")
	UMaterialInterface* ReadyState;

	UPROPERTY(EditAnyWhere, Category = "Materials")
	UMaterialInterface* NotReadyState;
	
    UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* OverheadPlayerSpot;

public:	
	TArray<UStaticMeshComponent*> Components;
};
