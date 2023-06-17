// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSpot.h"

FPlayerSpot::FPlayerSpot()
{

}

FPlayerSpot::FPlayerSpot(int32 Index, FVector Location, FRotator Rotation)
{
    this->Index = Index;
    this->Location = Location;
    this->Rotation = Rotation;
}

FPlayerSpot::~FPlayerSpot()
{
}
