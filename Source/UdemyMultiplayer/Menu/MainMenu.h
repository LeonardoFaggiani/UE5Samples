// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MenuBase.h"
#include "CommonButtonBase.h"
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

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
		TObjectPtr<UCommonButtonBase> HostButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
		TObjectPtr<UCommonButtonBase> FindGamesButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
		TObjectPtr<UCommonButtonBase> ExitButton;

	UFUNCTION() void OnHostButtonClicked();
	UFUNCTION() void OnFindGamesButtonClick();
	UFUNCTION() void OnExitButtonClick();

private:
	class UUdemyMultiplayerGameInstance* UdemyMultiplayerGameInstance;
};
