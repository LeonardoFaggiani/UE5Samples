// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "PlayerLobbyList.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYMULTIPLAYER_API UPlayerLobbyList : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetPlayerName(FString playerName);
	void SetIsReady(bool bIsReady);

	UPROPERTY(Replicated, meta = (BindWidget))
		FString PlayerStatus;

	UPROPERTY(Replicated, meta = (BindWidget))
		FString PlayerName;

private:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PlayerNameText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PlayerStatusText;
};
