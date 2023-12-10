// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuBase.h"
#include "Components/Button.h"
#include "CommonButtonBase.h"
#include "SliderBase.h"
#include "Components/TextBlock.h" 
#include "../UdemyMultiplayerGameInstance.h"
#include "OptionsMenu.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYMULTIPLAYER_API UOptionsMenu : public UMenuBase
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
		TObjectPtr<UCommonButtonBase> ApplyButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
		TObjectPtr<UCommonButtonBase> BackButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* PreviousTextureQualityButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* NextTextureQualityButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* PreviousResolutionButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* NextResolutionButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* PreviousWindowModeButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* NextWindowModeButton;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextureOption;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ScreenResolutionOption;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* WindowModeOption;

	UFUNCTION() void OnPreviousTextureQualityButtonClicked();
	UFUNCTION() void OnNextTextureQualityButtonClicked();
	UFUNCTION() void OnPreviousResolutionButtonClicked();
	UFUNCTION() void OnNextResolutionButtonClicked();
	UFUNCTION() void OnPreviousWindowModeButtonClicked();
	UFUNCTION() void OnNextWindowModeButtonClicked();

	UFUNCTION() void OnApplyButtonClicked();
	UFUNCTION() void OnBackButtonClicked();

	UFUNCTION(BlueprintCallable) void SetTextureQuality(int32 InTextureQuality);
	UFUNCTION(BlueprintCallable) void SetScreenResolution(FIntPoint InScreenResolution);
	UFUNCTION(BlueprintCallable) void SetWindowMode(EWindowMode::Type InWindowMode);

private:
	TMap<int32,FText> TextureQualityMaps;
	TMap<EWindowMode::Type, FText> WindowModeMaps;
	TMap<int32, EWindowMode::Type> WindowModeIndexMaps;
	TMap<int32, FText> SupportedResolutionsMaps;
	TArray<FIntPoint> SupportedResolutions;
	class UUdemyMultiplayerGameInstance* UdemyMultiplayerGameInstance;

private:
	void SetDefaultGameSettings();
	void InitializeSettings();
	void InitializeScreenResolutionSupported();	
	int32 GetTextureIdByValueSelected();
	FIntPoint GetScreenResolutionByValueSelected();
	EWindowMode::Type GetWindowModeByValueSelected();
};
