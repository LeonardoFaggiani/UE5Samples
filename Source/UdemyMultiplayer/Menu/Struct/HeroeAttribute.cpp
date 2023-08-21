// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroeAttribute.h"

FHeroeAttribute::FHeroeAttribute(FString InIcon,
    TEnumAsByte<EAttributeType> InAttributeType,
    int32 InPoints)
{
    this->Icon = InIcon;
    this->AttributeType = InAttributeType;
    this->Points = InPoints;
}

FHeroeAttribute::FHeroeAttribute()
{
}

FHeroeAttribute::~FHeroeAttribute()
{

}
