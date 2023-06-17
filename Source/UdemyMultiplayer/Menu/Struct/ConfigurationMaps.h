// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConfigurationMaps.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct UDEMYMULTIPLAYER_API FConfigurationMaps
{
public:
	GENERATED_BODY()

		UPROPERTY(BlueprintReadOnly)
		FString Path;
		UPROPERTY(BlueprintReadOnly)
		FString ImagePath;
		UPROPERTY(BlueprintReadOnly)
		FString Name;
		UPROPERTY(BlueprintReadOnly)
		int32 Order;

	FConfigurationMaps();
	FConfigurationMaps(FString Path, FString ImagePath, FString Name, int32 Order);
	~FConfigurationMaps();
};
