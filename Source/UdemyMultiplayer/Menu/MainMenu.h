// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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

protected:
	virtual bool Initialize();
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
		class UButton* ExitButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* FindGamesButton;

	UFUNCTION() void OnHostButtonClicked();
	UFUNCTION() void OnFindGamesButtonClick();
	UFUNCTION() void OnExitButtonClick();

private:
	class UUdemyMultiplayerGameInstance* UdemyMultiplayerGameInstance;
};
