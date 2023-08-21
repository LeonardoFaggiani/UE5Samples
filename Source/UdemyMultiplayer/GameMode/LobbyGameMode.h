// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "../LobbyPlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "../Menu/Struct/LobbyPlayerInfo.h"
#include "../UdemyMultiplayerCharacter.h"
#include "../Menu/LobbyPlayerSpot.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYMULTIPLAYER_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ALobbyGameMode(const FObjectInitializer& ObjectInitializer);
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	void LaunchTheGame();

	bool IsAllPlayerReady();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_SwapCharacter(APlayerController* PlayerController, TSubclassOf<AUdemyMultiplayerCharacter> InHeroeSelected,  bool bChangeStatus);
	void Server_SwapCharacter_Implementation(APlayerController* PlayerController, TSubclassOf<AUdemyMultiplayerCharacter> InHeroeSelected, bool bChangeStatus);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_UpdateGameSettings(UTexture2D* mapImage, const FString& mapName);
	void Server_UpdateGameSettings_Implementation(UTexture2D* mapImage, const FString& mapName);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_RespawnPlayer(ALobbyPlayerController* LobbyPlayerController);
	void Server_RespawnPlayer_Implementation(ALobbyPlayerController* LobbyPlayerController);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_EveryoneUpdate();
	void Server_EveryoneUpdate_Implementation();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_SpawnLobbyPlayerSpot();
	void Server_SpawnLobbyPlayerSpot_Implementation();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_UpdatePlayerName();
	void Server_UpdatePlayerName_Implementation();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_SetViewTargetSpot();
	void Server_SetViewTargetSpot_Implementation();

	FPlayerSpot* GetPlayerSpotForPlayerConnected();

	UPROPERTY(BlueprintReadOnly, Replicated)
	TArray<class ALobbyPlayerController*> AllPlayerControllers;
	UPROPERTY(Replicated)
	TArray<struct FLobbyPlayerInfo> ConnectedPlayers;
	UPROPERTY(Replicated)
	UTexture2D* MapImage;
	UPROPERTY(Replicated)
	FString MapName;
	UPROPERTY(Replicated)
	int32 CurrentPlayers;
	UPROPERTY(Replicated)
	int32 MaxPlayers;
	TSubclassOf<AUdemyMultiplayerCharacter> DefaultCharacterChampion;
	UPROPERTY(EditAnyWhere)
	TSubclassOf<ALobbyPlayerSpot> LobbyPlayerSpotClass;

private:
	void DestroyCharacterSelectedIfExits(ALobbyPlayerController* LobbyPlayerController);
	void SpawnCharacterOnPlayerSpot(ALobbyPlayerController* LobbyPlayerController);
	class UUdemyMultiplayerGameInstance* UdemyMultiplayerGameInstance;
};
