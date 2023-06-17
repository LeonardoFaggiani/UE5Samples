// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "MenuBase.h"
#include "MultiplayerSessions/public/MultiplayerSessionsSubsystem.h"
#include "../UdemyMultiplayerGameInstance.h"
#include "CreateSession.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYMULTIPLAYER_API UCreateSession : public UMenuBase
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;

	UFUNCTION()
		void OnCreateSession(bool bWasSuccessful);
	UFUNCTION()
		void OnStartSession(bool bWasSuccessful);	

private:

	UPROPERTY(meta = (BindWidget))
		class UButton* HostLobby;

	UPROPERTY(meta = (BindWidget))
		class UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* DecrementPlayerCountButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* IncrementPlayerCountButton;

	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* ServerNameTextBox;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PlayersCountText;

	UPROPERTY(meta = (BindWidget))
		class UCheckBox* IsLan;

	UFUNCTION() void OnHostLobbyButtonClicked();
	UFUNCTION() void OnBackButtonClicked();
	UFUNCTION() void OnDecrementPlayerCountButtonClick();
	UFUNCTION() void OnIncrementPlayerCountButtonClick();

	int32 NumPublicConnections{ 4 };
	FString MatchType{ TEXT("FreeForAll") };
	FString PathToLobby = FString::Printf(TEXT("/Game/ThirdPerson/Maps/Lobby?listen"));
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;
	UUdemyMultiplayerGameInstance* UdemyMultiplayerGameInstance;
};
