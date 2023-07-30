// Fill out your copyright notice in the Description page of Project Settings.

#include "FIndGames.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include <UdemyMultiplayer/UdemyMultiplayerGameInstance.h>


void UFindGames::NativeConstruct()
{
    Super::NativeConstruct();

    UGameInstance* GameInstance = GetGameInstance();

    if (GameInstance)
    {
        MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
    }

    if (MultiplayerSessionsSubsystem) {
        MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &UFindGames::OnFindSessions);
    }

    if (BackButton && RefreshButton) {
        BackButton->OnClicked.AddDynamic(this, &UFindGames::OnBackButtonClick);
        RefreshButton->OnClicked.AddDynamic(this, &UFindGames::OnRefreshButtonButtonClick);
    }

    if (UScrollBox* ScrollBox = Cast<UScrollBox>(GetWidgetFromName("ServerListScrollBox")))
    {
        ItemWidgetsBox = NewObject<UVerticalBox>();
        ScrollBox->AddChild(ItemWidgetsBox);
    }
    else
    {
        // Can't find the scroll widget, wrong name maybe, or a widget with that name is not a scrollwidget
        ServerListScrollBox = nullptr;
    }
}

void UFindGames::NativeDestruct()
{
    TearDown();

    Super::NativeDestruct();
}

#pragma region 
void UFindGames::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
    RefreshButton->SetIsEnabled(false);

    if (MultiplayerSessionsSubsystem == nullptr)
        return;

    if (!ServerListScrollBox)
    {
        return;
    }

    ServerListScrollBox->ClearChildren();
    int32 SessionIndex = 0;

    for (auto Result : SessionResults)
    {
        FString SettingsValue;
        Result.Session.SessionSettings.Get(FName("MatchType"), SettingsValue);

        if (SettingsValue == MatchType)
        {
            FString PlayerName = Result.Session.OwningUserName;
            int Ping = Result.PingInMs;
            FString ServerName;
            Result.Session.SessionSettings.Get(FName("ServerName"), ServerName);

            USessionList* SessionList = CreateWidget<USessionList>(this, SessionListClass);
            SessionList->SetPlayerName(PlayerName.Left(10));
            SessionList->SetServerName(ServerName);
            SessionList->SetPing(Ping);
            SessionList->SetSessionIndex(SessionIndex);

            ServerListScrollBox->AddChild(SessionList);

            SessionIndex++;
        }
    }

    if (!bWasSuccessful || SessionResults.Num() == 0)
    {
        SetStatusMessage(FString::Printf(TEXT("No sessions found.")));
    }

    RefreshButton->SetIsEnabled(true);
    ShowLoader(false);
}
#pragma endregion FindSessions

void UFindGames::SetStatusMessage(FString Message)
{
    StatusMessage->SetText(FText::FromString(Message));
}

void UFindGames::ShowLoader(bool IsShow)
{
    ESlateVisibility state = IsShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
    SearchingLoader->SetVisibility(state);
}

void UFindGames::OnBackButtonClick()
{

    UGameInstance* GameInstance = GetGameInstance();

    if (GameInstance != nullptr) {

        UUdemyMultiplayerGameInstance* UdemyMultiplayerGameInstance = Cast<UUdemyMultiplayerGameInstance>(GameInstance);

        if (UdemyMultiplayerGameInstance != nullptr)
        {
            TearDown();
            UdemyMultiplayerGameInstance->LoadMainMenu();
        }
    }    
}

void UFindGames::OnRefreshButtonButtonClick()
{
    RefreshButton->SetIsEnabled(false);

    ServerListScrollBox->ClearChildren();
    ShowLoader(true);
    SetStatusMessage(FString::Printf(TEXT("Searching...")));

    UGameInstance* GameInstance = GetGameInstance();

    if (GameInstance != nullptr) {

        UUdemyMultiplayerGameInstance* UdemyMultiplayerGameInstance = Cast<UUdemyMultiplayerGameInstance>(GameInstance);

        if (UdemyMultiplayerGameInstance != nullptr)
        {
            ECheckBoxState CheckBoxState = this->IsLan->GetCheckedState();

            UdemyMultiplayerGameInstance->Join();
        }
    }
}