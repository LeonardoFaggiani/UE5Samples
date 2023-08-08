// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Menu/Lobby.h"
#include "Menu/CharacterSelection.h"
#include "Blueprint/UserWidget.h"
#include "Components/Widget.h"
#include "Components/CanvasPanelSlot.h"
#include "./Menu/Struct/LobbyPlayerInfo.h"
#include "UdemyMultiplayerGameInstance.h"
#include "InputActionValue.h"
#include "Templates/SharedPointer.h"
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
	void UpdateReadyState();
	ALobbyPlayerSpot* GetPlayerSpot();

	UPROPERTY(Replicated)
	FLobbyPlayerInfo PlayerSettings;

	UPROPERTY(Replicated)
	ALobbyPlayerSpot* PlayerSpot;

	UFUNCTION(BlueprintCallable, Client, Reliable)
		void Client_SetupLobbyMenu(const FString& ServerName);
		void Client_SetupLobbyMenu_Implementation(const FString& ServerName);

	UFUNCTION(BlueprintCallable, Client, Reliable)
		void Client_UpdateLobbySettings(UTexture2D* MapImage, const FString& MapName);
		void Client_UpdateLobbySettings_Implementation(UTexture2D* MapImage, const FString& MapName);

	UFUNCTION(BlueprintCallable, Server, Reliable)
		void Server_CallUpdate(const FLobbyPlayerInfo& PlayerInfo);
		void Server_CallUpdate_Implementation(const FLobbyPlayerInfo& PlayerInfo);

	UFUNCTION(BlueprintCallable, Server, Reliable)
		void Server_NotifyPlayerStatus(const FLobbyPlayerInfo& PlayerInfo);
		void Server_NotifyPlayerStatus_Implementation(const FLobbyPlayerInfo& PlayerInfo);

	UFUNCTION(BlueprintCallable, Client, Reliable)
		void Client_UpdateNumberOfPlayers(int32 CurrentPlayers, int32 MaxPlayers);
		void Client_UpdateNumberOfPlayers_Implementation(int32 CurrentPlayers, int32 MaxPlayers);

	UFUNCTION(BlueprintCallable, Client, Reliable)
		void Client_AssignPlayer(int32 CharacterSelected);
		void Client_AssignPlayer_Implementation(int32 CharacterSelected);

	UFUNCTION(BlueprintCallable, Client, Reliable)
		void Client_ShowLoadingScreen();
		void Client_ShowLoadingScreen_Implementation();

		UFUNCTION(BlueprintCallable, Client, Reliable)
		void Client_SetViewTargetSpot();
		void Client_SetViewTargetSpot_Implementation();

	void ToggleCharacterSelectionMenu(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* LobbyPlayerControllerMappingContext;		

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* ToggleMenuAction;

		

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	class ULobby* Lobby;
	class UCharacterSelection* CharacterSelection;	
	class ALobbyGameMode* LobbyGameMode;
	class UUdemyMultiplayerGameInstance* UdemyMultiplayerGameInstance;
	AActor* ViewTarget;
	AActor* GetActorByName(FString InActorName);

	AUdemyMultiplayerCharacter* CurrentCharacter;
	TSubclassOf<UUserWidget> LobbyClass;
	TSubclassOf<UUserWidget> CharacterSelectionClass;
};
