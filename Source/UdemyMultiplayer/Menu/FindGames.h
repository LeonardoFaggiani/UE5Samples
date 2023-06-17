// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionList.h"
#include "Components/ScrollBox.h"
#include "Components/CircularThrobber.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/CheckBox.h"
#include "Components/Button.h"
#include "MultiplayerSessions/public/MultiplayerSessionsSubsystem.h"
#include "MenuBase.h"
#include "FIndGames.generated.h"


/**
 * 
 */
UCLASS()
class UDEMYMULTIPLAYER_API UFindGames : public UMenuBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);

public:

	UPROPERTY(EditAnyWhere)
		TSubclassOf<USessionList> SessionListClass;

	void SetStatusMessage(FString Message);
	void ShowLoader(bool IsShow);

private:

	FString MatchType{ TEXT("FreeForAll") };
	UVerticalBox* ItemWidgetsBox;

	UPROPERTY(meta = (BindWidget))
		UCircularThrobber* SearchingLoader;

	UPROPERTY(meta = (BindWidget))
		UScrollBox* ServerListScrollBox;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* StatusMessage;

	UPROPERTY(meta = (BindWidget))
		UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
		UButton* RefreshButton;

	UPROPERTY(meta = (BindWidget))
		class UCheckBox* IsLan;

	UFUNCTION() void OnBackButtonClick();
	UFUNCTION() void OnRefreshButtonButtonClick();

	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;
};
