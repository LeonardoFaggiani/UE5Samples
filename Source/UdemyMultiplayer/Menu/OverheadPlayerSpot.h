// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "OverheadPlayerSpot.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYMULTIPLAYER_API UOverheadPlayerSpot : public UUserWidget
{

	GENERATED_BODY()

public:
	void SetPlayerName(FString PlayerName);
	void SetReadyStatus(bool bIsReady);
	void SetReadyStatusVisibility(bool bIsHidden);
	void SetPlayerNameColor(FString Color);

	UPROPERTY(meta = (BindWidget))
		UTextBlock* LobbyPlayerName;

	UPROPERTY(meta = (BindWidget))
		UImage* ReadyStatusIcon;

protected:
	virtual void NativeDestruct() override;		
};
