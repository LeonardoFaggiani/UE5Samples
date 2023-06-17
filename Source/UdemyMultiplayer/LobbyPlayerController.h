// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Menu/Lobby.h"
#include "Blueprint/UserWidget.h"
#include "./Menu/Struct/LobbyPlayerInfo.h"
#include "InputActionValue.h"
#include "LobbyPlayerController.generated.h"

class UInputMappingContext;
class AUdemyMultiplayerCharacter;
class ALobbyPlayerSpot;

/**
 * 
 */
UCLASS()
class UDEMYMULTIPLAYER_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ALobbyPlayerController(const FObjectInitializer& ObjectInitializer);
	void SetCurrentCharacter(AUdemyMultiplayerCharacter* currentCharacter);
	AUdemyMultiplayerCharacter* GetCurrentCharacter();
	void SetPlayerSpot(ALobbyPlayerSpot* playerSpot);
	ALobbyPlayerSpot* GetPlayerSpot();

	UPROPERTY(Replicated)
	FLobbyPlayerInfo PlayerSettings;

	UPROPERTY(Replicated)
	TArray<struct FLobbyPlayerInfo> AllConnectedPlayers;

	UFUNCTION(BlueprintCallable, Client, Reliable)
		void Client_SetupLobbyMenu(const FString& ServerName);
		void Client_SetupLobbyMenu_Implementation(const FString& ServerName);

	UFUNCTION(BlueprintCallable, Client, Reliable)
		void Client_UpdateLobbySettings(UTexture2D* MapImage, const FString& MapName);
		void Client_UpdateLobbySettings_Implementation(UTexture2D* MapImage, const FString& MapName);

	UFUNCTION(BlueprintCallable, Server, Reliable)
		void Server_CallUpdate(const FLobbyPlayerInfo& PlayerInfo);
		void Server_CallUpdate_Implementation(const FLobbyPlayerInfo& PlayerInfo);

	UFUNCTION(BlueprintCallable, Client, Reliable)
		void Client_UpdateNumberOfPlayers(int32 CurrentPlayers, int32 MaxPlayers);
		void Client_UpdateNumberOfPlayers_Implementation(int32 CurrentPlayers, int32 MaxPlayers);

	UFUNCTION(BlueprintCallable, Client, Reliable)
		void Client_AssignPlayer(int32 CharacterSelected);
		void Client_AssignPlayer_Implementation(int32 CharacterSelected);

	UFUNCTION(BlueprintCallable, Client, Reliable)
		void Client_AddPlayersInfo(const TArray<struct FLobbyPlayerInfo>& ConnectedPlayersInfo);
		void Client_AddPlayersInfo_Implementation(const TArray<struct FLobbyPlayerInfo>& ConnectedPlayersInfo);

	UFUNCTION(BlueprintCallable, Client, Reliable)
		void Client_ShowLoadingScreen();
		void Client_ShowLoadingScreen_Implementation();

private:
	class ULobby* Lobby;
	class ALobbyGameMode* LobbyGameMode;
	AUdemyMultiplayerCharacter* CurrentCharacter;
	ALobbyPlayerSpot* PlayerSpot;
	TSubclassOf<UUserWidget> LobbyClass;
};
