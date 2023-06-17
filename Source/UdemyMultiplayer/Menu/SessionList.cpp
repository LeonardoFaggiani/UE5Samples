// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionList.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "OnlineSessionSettings.h"
#include "../LobbyPlayerController.h"
#include "OnlineSubsystem.h"
#include <string>

void USessionList::InitializeSubSystem() {

    UGameInstance* GameInstance = GetGameInstance();

    if (GameInstance)
    {
        MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
        UdemyMultiplayerGameInstance = Cast<UUdemyMultiplayerGameInstance>(GameInstance);
    }

    if (MultiplayerSessionsSubsystem) {
        MultiplayerSessionsSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &USessionList::OnJoinSession);
    }
}

bool USessionList::Initialize()
{
    if (!Super::Initialize())
        return false;

    InitializeSubSystem();

    if (JoinButton)
    {
        JoinButton->OnClicked.AddDynamic(this, &ThisClass::OnJoinButtonClicked);
    }

    return true;
}

void USessionList::NativeDestruct()
{
    Super::NativeDestruct();
}

#pragma region 

void USessionList::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
        if (SessionInterface.IsValid())
        {
            FString Address;
            SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);

            APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();

            ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(PlayerController);

            if (LobbyPlayerController)
            {
                LobbyPlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
            }
        }
    }
}
#pragma endregion JoinSessions

void USessionList::SetSessionIndex(int32 Index)
{
    FString SessionIn = FString::FromInt(Index);

    SessionIndex = Index;
}

void USessionList::SetPing(int Ping)
{
    PingText->SetText(FText::FromString(FString::FromInt(Ping)));
}

void USessionList::SetServerName(FString ServerName)
{
    ServerNameText->SetText(FText::FromString(ServerName));
}

void USessionList::SetPlayerName(FString PlayerName)
{
    PlayerNameText->SetText(FText::FromString(PlayerName));
}

void USessionList::EnableJoinButton(bool IsEnable)
{
    JoinButton->SetIsEnabled(IsEnable);
}

void USessionList::OnJoinButtonClicked()
{
    this->EnableJoinButton(false);

    this->UdemyMultiplayerGameInstance->ShowLoadingScreen();

    if (MultiplayerSessionsSubsystem) {

        TSharedPtr<FOnlineSessionSearch> LastSessionsSearch = MultiplayerSessionsSubsystem->GetLastSessionsSearch();

        if (LastSessionsSearch.IsValid()) {

            FString SessionIn = FString::FromInt(this->SessionIndex);

            FString CountSession = FString::FromInt(LastSessionsSearch->SearchResults.Num());

            MultiplayerSessionsSubsystem->JoinSession(LastSessionsSearch->SearchResults[this->SessionIndex]);
        }
    }
}