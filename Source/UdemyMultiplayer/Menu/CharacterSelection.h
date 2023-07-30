// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MenuBase.h"
#include "../UdemyMultiplayerCharacter.h"
#include "Animation/WidgetAnimation.h"
#include "CharacterSelection.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYMULTIPLAYER_API UCharacterSelection : public UMenuBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(Replicated)
	int32 CurrentCharacterSelected;

	void ToggleMenu();
	void SetButtonEnabled(int32 InButtonIndexPressed);

protected:
	virtual bool Initialize() override;	


private:
	UPROPERTY(meta = (BindWidget))
		UButton* ArcherButton;

	UPROPERTY(meta = (BindWidget))
		UButton* WarriorButton;

	UPROPERTY(meta = (BindWidget))
		UButton* WizardButton;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* FadeMenu;

	UFUNCTION() void OnArcherButtonClicked();
	UFUNCTION() void OnWarriorButtonClicked();
	UFUNCTION() void OnWizardButtonClicked();

	void NotifyPlayerController();

	AUdemyMultiplayerCharacter* WarriorCharacter;
	AUdemyMultiplayerCharacter* ArcherCharacter;
	AUdemyMultiplayerCharacter* WizardCharacter;

private:
	float CurrentOpacity;
	FWidgetAnimationDynamicEvent StartDelegate;
	FWidgetAnimationDynamicEvent EndDelegate;
	UFUNCTION() void AnimationStarted();
	UFUNCTION() void AnimationFinished();
};
