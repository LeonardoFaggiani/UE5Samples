// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerSpot.h"

// Sets default values
ALobbyPlayerSpot::ALobbyPlayerSpot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

bool ALobbyPlayerSpot::GetIsUsed()
{
	return this->IsUsed;
}

void ALobbyPlayerSpot::SetIsUsed(bool isUsed)
{
	this->IsUsed = isUsed;
}

// Called when the game starts or when spawned
void ALobbyPlayerSpot::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALobbyPlayerSpot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

