// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "HeroeItem.h"
#include "MenuBase.h"
#include "../UdemyMultiplayerCharacter.h"
#include "HeroeSelection.generated.h"


/**
 * 
 */
UCLASS()
class UDEMYMULTIPLAYER_API UHeroeSelection : public UMenuBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UHeroeItem> HeroeItem;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
		TObjectPtr<UCommonButtonBase> BackButton;

	UPROPERTY(meta = (BindWidget))
		UHorizontalBox* Container;

	void FillContainer();

protected:
	virtual bool Initialize() override;	

private:
	UFUNCTION() void OnBackButtonClicked();

private:
	class UUdemyMultiplayerGameInstance* UdemyMultiplayerGameInstance;

};
