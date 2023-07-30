// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Animation/WidgetAnimation.h"
#include "MenuBase.h"
#include "LoadingScreen.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYMULTIPLAYER_API ULoadingScreen : public UMenuBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void TransBounceIn();
	UFUNCTION(BlueprintCallable)
	void TransBounceOut();

private:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* BounceInOut;
};
