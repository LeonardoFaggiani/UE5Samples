// Fill out your copyright notice in the Description page of Project Settings.


#include "UdemyMultiplayerPlayerState.h"


AUdemyMultiplayerPlayerState::AUdemyMultiplayerPlayerState() {

}

void AUdemyMultiplayerPlayerState::AddHeroeItem(UHeroeItem* HeroeItem)
{
    this->HeroesItems.Add(HeroeItem);
}

TArray<UHeroeItem*> AUdemyMultiplayerPlayerState::GetHeroesItems()
{
    return this->HeroesItems;
}