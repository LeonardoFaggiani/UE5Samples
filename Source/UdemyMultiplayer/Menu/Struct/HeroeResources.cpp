// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroeResources.h"
#include "HeroeAttribute.h"


FHeroeResources::FHeroeResources(FString InName,
    TSubclassOf<AUdemyMultiplayerCharacter> InTargetClass,
    FString InHeroeIcon,
    TArray<FHeroeAttribute> InHeroeAttribute)
{
    this->Name = InName;
    this->TargetClass = InTargetClass;
    this->HeroeIcon = InHeroeIcon;
    this->HeroeAttributes = InHeroeAttribute;
}

FHeroeResources::FHeroeResources()
{
}

FHeroeResources::~FHeroeResources()
{
}
