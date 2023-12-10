// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "../UdemyMultiplayerCharacter.h"
#include "OverheadPlayerSpot.generated.h"


/**
 * 
 */
UCLASS()
class UDEMYMULTIPLAYER_API UOverheadPlayerSpot : public UUserWidget
{

	GENERATED_BODY()

public:

	void UpdateReadyStatus();
	void UpdatePlayerName();
	void SetCharacter(AUdemyMultiplayerCharacter* InUdemyMultiplayerCharacter);

	void SetReadyStatusVisibility(bool bIsHidden);
	void SetPlayerNameColor(FString Color);

	UFUNCTION(BlueprintCallable)
		UImage* GetStatusIcon();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* LobbyPlayerName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* ReadyStatusIcon;

	AUdemyMultiplayerCharacter* UdemyMultiplayerCharacter;

protected:
	virtual void NativeDestruct() override;		
};
