// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "MenuBase.h"
#include "OptionsMenu.h"
#include "CommonButtonBase.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYMULTIPLAYER_API UInGameMenu : public UMenuBase
{
	GENERATED_BODY()
	
	virtual bool Initialize();
	virtual void NativeDestruct() override;

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
		TObjectPtr<UCommonButtonBase> OptionsButton;

	UPROPERTY(EditAnyWhere)
		TSubclassOf<UOptionsMenu> OptionsMenuClass;

private:

	class UOptionsMenu* OptionsMenu;

	UFUNCTION() void OnOptionsButtonClicked();

};
