// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Menu/MenuInterface.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerSessions/public/MultiplayerSessionsSubsystem.h"
#include "Menu/Struct/ConfigurationMaps.h"
#include "Menu/Struct/PlayerSpot.h"
#include "Menu/MainMenu.h"
#include "Menu/LoadingScreen.h"
#include "UdemyMultiplayerGameInstance.generated.h"


/**
 * 
 */
UCLASS()
class UDEMYMULTIPLAYER_API UUdemyMultiplayerGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:

	UUdemyMultiplayerGameInstance(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
		void LoadMenu();

	UFUNCTION(Exec)
		virtual void Host() override;

	UFUNCTION(Exec)
		virtual void Join(bool bIsLan) override;

	UFUNCTION(Exec)
		virtual void Quit() override;

	UFUNCTION(Exec)
		virtual void GoToLobby() override;

	UFUNCTION(Exec)
		virtual void LoadMainMenu() override;

	UFUNCTION(BlueprintCallable)
		void ShowLoadingScreen();

	void InitializeMapConfigurations();
	void InitializePlayerSpot();
	void SetHostSettings(int32 NumberOfPlayers, bool bIsLan, FString ServerName);

    TSubclassOf<UUserWidget> MenuClass;
	TSubclassOf<UUserWidget> LoadingScreenClass;

    TMap<FString, FConfigurationMaps> ConfigurationMaps;
	TMap<int32, FPlayerSpot> ConfigurationPlayerSpot;

    UPROPERTY(Replicated)
        int32 MaxPlayers;
    UPROPERTY(Replicated)
        FString ServerName;
    bool bIsLAN{ false };

private:

	class UMainMenu* Menu;
	class ULoadingScreen* LoadingScreen;

	void SetSwitchByIndex(int32 Index);

	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;
};
