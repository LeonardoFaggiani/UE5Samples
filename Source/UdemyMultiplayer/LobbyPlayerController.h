// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/Widget.h"
#include "UdemyMultiplayerGameInstance.h"
#include "./Menu/Struct/LobbyPlayerInfo.h"
#include "Menu/Lobby.h"
#include "Menu/HeroeSelection.h"
#include "LobbyPlayerController.generated.h"


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

    UPROPERTY(EditAnyWhere)
        TSubclassOf<ULobby> LobbyClass;

	UPROPERTY(BlueprintReadWrite, Replicated)
		FLobbyPlayerInfo PlayerSettings;

	UPROPERTY(BlueprintReadWrite, Replicated)
		ALobbyPlayerSpot* PlayerSpot;

	UFUNCTION(BlueprintCallable, Server, Reliable)
		void Server_CallUpdate(const FLobbyPlayerInfo& PlayerInfo);
		void Server_CallUpdate_Implementation(const FLobbyPlayerInfo& PlayerInfo);

	UFUNCTION(BlueprintCallable, Server, Reliable)
		void Server_NotifyPlayerStatus(const FLobbyPlayerInfo& PlayerInfo);
		void Server_NotifyPlayerStatus_Implementation(const FLobbyPlayerInfo& PlayerInfo);

	UFUNCTION(BlueprintCallable, Client, Reliable)
		void Client_SetupLobbyMenu(const FString& ServerName);
		void Client_SetupLobbyMenu_Implementation(const FString& ServerName);

	UFUNCTION(BlueprintCallable, Client, Reliable)
		void Client_UpdateLobbySettings(UTexture2D* MapImage, const FString& MapName);
		void Client_UpdateLobbySettings_Implementation(UTexture2D* MapImage, const FString& MapName);

	UFUNCTION(BlueprintCallable, Client, Reliable)
		void Client_UpdateNumberOfPlayers(int32 CurrentPlayers, int32 MaxPlayers);
		void Client_UpdateNumberOfPlayers_Implementation(int32 CurrentPlayers, int32 MaxPlayers);

	UFUNCTION(BlueprintCallable, Client, Reliable)
		void Client_AssignHeroeToPlayer(TSubclassOf<AUdemyMultiplayerCharacter> HeroeClass);
		void Client_AssignHeroeToPlayer_Implementation(TSubclassOf<AUdemyMultiplayerCharacter> HeroeClass);

	UFUNCTION(BlueprintCallable, Client, Reliable)
		void Client_ShowLoadingScreen();
		void Client_ShowLoadingScreen_Implementation();

	UFUNCTION(BlueprintCallable, Client, Reliable)
		void Client_SetViewTargetSpot();
		void Client_SetViewTargetSpot_Implementation();

	UFUNCTION(BlueprintCallable, Client, Reliable)
		void Client_SwitchToLobbyMode();
		void Client_SwitchToLobbyMode_Implementation();

private:
	class ULobby* Lobby;
	class UHeroeSelection* HeroeSelection;
	class ALobbyGameMode* LobbyGameMode;
	class UUdemyMultiplayerGameInstance* UdemyMultiplayerGameInstance;
	AActor* ViewTarget;
	AActor* GetActorByName(FString InActorName);

	AUdemyMultiplayerCharacter* CurrentCharacter;
};
