// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "MenuBase.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYMULTIPLAYER_API UMainMenu : public UMenuBase
{
	GENERATED_BODY()

public:
	UWidgetSwitcher* UMainMenu::GetMenuSwitcher();

protected:
	virtual bool Initialize();
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
		class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* FindGamesButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* ExitButton;

	UFUNCTION() void OnHostButtonClicked();
	UFUNCTION() void OnFindGamesButtonClick();
	UFUNCTION() void OnExitButtonClick();

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* WidgetSwitcherMenu;

};
