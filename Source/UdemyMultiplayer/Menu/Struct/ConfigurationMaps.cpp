// Fill out your copyright notice in the Description page of Project Settings.


#include "ConfigurationMaps.h"

FConfigurationMaps::FConfigurationMaps()
{

}

FConfigurationMaps::FConfigurationMaps(FString Path, FString ImagePath, FString Name, int32 Order)
{
    this->Path = Path;
    this->ImagePath = ImagePath;
    this->Name = Name;
    this->Order = Order;
}

FConfigurationMaps::~FConfigurationMaps()
{

}
