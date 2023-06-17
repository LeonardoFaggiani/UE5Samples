// Fill out your copyright notice in the Description page of Project Settings.


#include "UdemyMultiplayerPlayerState.h"
#include "Net/UnrealNetwork.h"

AUdemyMultiplayerPlayerState::AUdemyMultiplayerPlayerState()
{
	this->bIsReady = false;
}

void AUdemyMultiplayerPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUdemyMultiplayerPlayerState, bIsReady);
}