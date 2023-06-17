// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "MenuBase.h"
#include "../Menu/Struct/LobbyPlayerInfo.h"
#include "../Menu/Struct/ConfigurationMaps.h"
#include "../UdemyMultiplayerGameInstance.h"
#include "PlayerLobbyList.h"
#include "Lobby.generated.h"


/**
 * 
 */
UCLASS()
class UDEMYMULTIPLAYER_API ULobby : public UMenuBase
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnyWhere)
		TSubclassOf<UPlayerLobbyList> PlayerLobbyListClass;

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_UpdatePlayerList(const FLobbyPlayerInfo& PlayerInfo);
	void Client_UpdatePlayerList_Implementation(const FLobbyPlayerInfo& PlayerInfo);

	UPROPERTY(meta = (BindWidget))
		class UOverlay* CharacterSelectionContainer;

	UPROPERTY(meta = (BindWidget))
		class UScrollBox* PlayerListInLobby;

	void SetCurrentPlayersFormat(FString currentPlayersFormat);
	void SetServerName(FString serverName);
	void UpdateStatus();
	void SetMap(UTexture2D* mapImage, FString mapName);
	void ShowOrHideButton();

	UFUNCTION()
		void ClearPlayerList();

protected:

	virtual bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CurrentPlayersFormat;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ServerName;

	UPROPERTY(meta = (BindWidget))
		class UButton* ReadyButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* PlayButton;
	
	UPROPERTY(Replicated, meta = (BindWidget))
		class UImage* MapImage;

	UPROPERTY(Replicated, meta = (BindWidget))
		class UTextBlock* MapName;

	UPROPERTY(meta = (BindWidget))
		class UButton* PreviousMap;

	UPROPERTY(meta = (BindWidget))
		class UButton* NextMap;

	UFUNCTION()
		void OnReadyButtonClicked();	
	UFUNCTION() 
		void OnPlayButtonClicked();
	UFUNCTION() 
		void OnPreviousMapButtonClicked();
	UFUNCTION() 
		void OnNextMapButtonClicked();

private:
	void InitializeMap();
	FConfigurationMaps* GetCurrentMapByName(FString Name);
	FConfigurationMaps* GetPreviousNextMap(bool IsIncrement);
	void NotifyMapChaged();
	UUdemyMultiplayerGameInstance* UdemyMultiplayerGameInstance;
	class ALobbyGameMode* LobbyGameMode;
	UTexture2D* CurrentTextureMap;
};