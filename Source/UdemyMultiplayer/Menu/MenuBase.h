// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MenuBase.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYMULTIPLAYER_API UMenuBase : public UUserWidget
{
	GENERATED_BODY()

public:

	void Setup();

	void TearDown();

	void SetMenuInterface(IMenuInterface* MenuInterface);

protected:

	IMenuInterface* MenuInterface;
};
