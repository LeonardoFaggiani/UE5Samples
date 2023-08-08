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
#include "Engine/LevelScriptActor.h"
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

	void Init() override;

	UFUNCTION(BlueprintCallable)
		UMainMenu* LoadMenu();

	UFUNCTION(Exec)
		virtual void Join() override;

	UFUNCTION(Exec)
		virtual void Quit() override;

	UFUNCTION(BlueprintCallable, Exec)
		virtual void LoadMainMenu() override;

	UFUNCTION(BlueprintCallable)
		void OpenNextLevel(FName InLevel, bool bIsListen, bool bShowLoading, float OpenLevelDelay);

	UFUNCTION(BlueprintCallable)
		void SetHostSettings(int32 NumberOfPlayers, FString ServerName);

	UFUNCTION(BlueprintCallable)
		void ShowLoadingScreen(bool bWithTransition);

	UFUNCTION(BlueprintCallable)
		void HideLoadingScreen(bool bWithTransition);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Loading Screen")
		void BeginLoadingScreen(const FString& MapName);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Loading Screen")
		void EndLoadingScreen(UWorld* InLoadedWorld);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loading Screen")
		TSubclassOf<class UUserWidget> LoadingScreenWidget;

	void InitializeMapConfigurations();
	void InitializePlayerSpot();

    TSubclassOf<UUserWidget> MenuClass;
	TSubclassOf<UUserWidget> LoadingScreenClass;

    TMap<FString, FConfigurationMaps> ConfigurationMaps;
	TMap<int32, FPlayerSpot> ConfigurationPlayerSpot;

    UPROPERTY(Replicated)
        int32 MaxPlayers;
    UPROPERTY(Replicated)
        FString ServerName;
	UPROPERTY(Replicated)
		TArray<TSubclassOf<class AUdemyMultiplayerCharacter>> Characters;

	UFUNCTION(BlueprintCallable)
		void SetBackToMainMenu(bool InbIsBackToMainMenu);
	UFUNCTION(BlueprintCallable)
		void SetHostGame(bool InbIsHostGameMenu);
	UFUNCTION(BlueprintCallable)
		void SetFindGames(bool InbIsFindGamesMenu);
	UFUNCTION(BlueprintCallable)
		void StopMovie();

	UFUNCTION(BlueprintCallable)
		bool GetBackToMainMenu();

	UFUNCTION(BlueprintCallable)
		bool GetFindGames();

	UFUNCTION(BlueprintCallable)
		bool GetHostGame();

private:
	UPROPERTY()
	bool bIsBackToMainMenu{ false };
	UPROPERTY()
	bool bIsHostGameMenu{ false };
	UPROPERTY()
	bool bIsFindGamesMenu{ false };
	UFUNCTION()
		void OpenLevelWithDelay(FName InLevelName, FString InListen);
	UPROPERTY()
		ULoadingScreen* LoadingScreen;

	class UMainMenu* Menu;
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;
};
