// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Menu/InGameMenu.h"
#include "InputActionValue.h"
#include "UdemyMultiplayerCharacter.h"
#include "InputAction.h"
#include "Net/UnrealNetwork.h"
#include "InGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYMULTIPLAYER_API AInGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnyWhere)
		TSubclassOf<UInGameMenu> InGameMenuClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputMappingContext* CommonPlayerControllerMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* ShowInGameMenuAction;

	void SetCurrentCharacter(AUdemyMultiplayerCharacter* InCurrentCharacter);
	AUdemyMultiplayerCharacter* GetCurrentCharacter();

private:
	void ShowInGameMenu(const FInputActionValue& Value);
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	class UInGameMenu* InGameMenu;
	AUdemyMultiplayerCharacter* CurrentCharacter;
};
