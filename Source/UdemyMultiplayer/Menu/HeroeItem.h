// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "CommonTextBlock.h"
#include "HeroeAttributeStats.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Struct/HeroeResources.h"
#include "Struct/HeroeAttribute.h"
#include "HeroeItem.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYMULTIPLAYER_API UHeroeItem : public UCommonButtonBase
{
	GENERATED_BODY()
	
public:

	UHeroeItem();
	void SetHeroeSelected();

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UHeroeAttributeStats> HeroeAttributeStats;

	UPROPERTY(meta = (BindWidget))
		UHorizontalBox* StatsContainer;	

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UCommonTextBlock> HeroeName;

	UPROPERTY(meta = (BindWidget))
		UImage* HeroeIcon;

	UPROPERTY(EditAnywhere, Category = "Heroe Settings")
		FString Name;

	UPROPERTY(EditAnywhere, Category = "Heroe Settings")
		FString IconPath;

	UFUNCTION(BlueprintCallable)
		void SetAttributeStats(const FHeroeResources& InHeroeResources);	

	UFUNCTION(BlueprintCallable)
		void SetHeroeName(FString InHeroeName);

	UFUNCTION(BlueprintCallable)
		void SetHeroeIcon(FString InHeroeIcon);
	
	UFUNCTION(BlueprintCallable)
		void RemoveDisabledStateToAllItems();
};