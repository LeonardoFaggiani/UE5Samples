// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "../UdemyMultiplayerCharacter.h"
#include "CharacterSelection.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYMULTIPLAYER_API UCharacterSelection : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(Replicated)
	int32 CurrentCharacterSelected;

protected:
	virtual bool Initialize() override;	

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* ArcherButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* WarriorButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* WizardButton;

	UFUNCTION() void OnArcherButtonClicked();
	UFUNCTION() void OnWarriorButtonClicked();
	UFUNCTION() void OnWizardButtonClicked();

	void NotifyPlayerController();

	AUdemyMultiplayerCharacter* WarriorCharacter;
	AUdemyMultiplayerCharacter* ArcherCharacter;
	AUdemyMultiplayerCharacter* WizardCharacter;
};
