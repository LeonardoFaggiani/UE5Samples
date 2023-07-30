// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
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

	UPROPERTY(meta = (BindWidget))
		UTextBlock* LobbyPlayerName;

protected:
	virtual void NativeDestruct() override;		
};
