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

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
		TObjectPtr<UCommonButtonBase> OptionsButton;

	UFUNCTION() void OnHostButtonClicked();
	UFUNCTION() void OnFindGamesButtonClicked();
	UFUNCTION() void OnOptionsButtonButtonClicked();
	UFUNCTION() void OnExitButtonClicked();


private:
	class UUdemyMultiplayerGameInstance* UdemyMultiplayerGameInstance;
};
