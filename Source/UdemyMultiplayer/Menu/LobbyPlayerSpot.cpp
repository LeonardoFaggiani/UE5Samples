// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerSpot.h"
#include "Net/UnrealNetwork.h"
#include "OverheadPlayerSpot.h"

// Sets default values
ALobbyPlayerSpot::ALobbyPlayerSpot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetIsReady(true);

	OverheadPlayerSpot = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadPlayerSpot"));
	OverheadPlayerSpot->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ALobbyPlayerSpot::BeginPlay()
{
	Super::BeginPlay();
}

void ALobbyPlayerSpot::SetIsReady(bool bready)
{
	this->bReady = bready;
}

void ALobbyPlayerSpot::OnRep_ReadyStateUpdated()
{
	this->GetComponents<UStaticMeshComponent>(Components);

	for (int32 i = 0; i < Components.Num(); i++)
	{
		UStaticMeshComponent* StaticMeshComponent = Components[i];

		if (StaticMeshComponent->GetName() == FString("PlayerStatus"))
		{
			UMaterialInterface* CurrentState = this->bReady ? ReadyState : NotReadyState;

			StaticMeshComponent->SetMaterial(0, CurrentState);
		}
	}
}

void ALobbyPlayerSpot::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerSpot, bReady);
}