// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerSessions/public/MultiplayerSessionsSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "../UdemyMultiplayerGameInstance.h"
#include "SessionList.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYMULTIPLAYER_API USessionList : public UUserWidget
{
	GENERATED_BODY()

public:

	void InitializeSubSystem();
	void SetPing(int Ping);
	void SetServerName(FString ServerName);
	void SetPlayerName(FString PlayerName);
	void SetSessionIndex(int32 SessionIndex);
	void EnableJoinButton(bool IsEnable);

protected:
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;

	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);

private:

	UPROPERTY(meta = (BindWidget))
		class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PingText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ServerNameText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PlayerNameText;

	UFUNCTION() void OnJoinButtonClicked();

	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;
	UUdemyMultiplayerGameInstance* UdemyMultiplayerGameInstance;
	int32 SessionIndex{ 0 };
	
};
