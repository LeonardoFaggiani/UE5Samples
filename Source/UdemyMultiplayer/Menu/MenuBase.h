// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuInterface.h"
#include "CommonUserWidget.h"
#include "MenuBase.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYMULTIPLAYER_API UMenuBase : public UCommonUserWidget
{
	GENERATED_BODY()

public:

	void Setup(int32 zOrder = 0);

	void TearDown();

	void SetMenuInterface(IMenuInterface* MenuInterface);

protected:

	IMenuInterface* MenuInterface;
};
