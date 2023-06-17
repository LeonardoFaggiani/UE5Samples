// Fill out your copyright notice in the Description page of Project Settings.

#include "CreateSession.h"
#include "Components/WidgetSwitcher.h"
#include "Styling/SlateTypes.h"
#include "Styling/SlateBrush.h"

bool UCreateSession::Initialize()
{
    if (!Super::Initialize())
        return false;


    UGameInstance* GameInstance = GetGameInstance();

    if (GameInstance) {
        MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
        UdemyMultiplayerGameInstance = Cast<UUdemyMultiplayerGameInstance>(GameInstance);
    }
        

    if (MultiplayerSessionsSubsystem) {
        MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &UCreateSession::OnCreateSession);
        MultiplayerSessionsSubsystem->MultiplayerOnStartSessionComplete.AddDynamic(this, &UCreateSession::OnStartSession);
    }

    if (HostLobby && BackButton && DecrementPlayerCountButton && IncrementPlayerCountButton) {

        HostLobby->OnClicked.AddDynamic(this, &UCreateSession::OnHostLobbyButtonClicked);
        BackButton->OnClicked.AddDynamic(this, &UCreateSession::OnBackButtonClicked);

        DecrementPlayerCountButton->OnClicked.AddDynamic(this, &UCreateSession::OnDecrementPlayerCountButtonClick);
        IncrementPlayerCountButton->OnClicked.AddDynamic(this, &UCreateSession::OnIncrementPlayerCountButtonClick);
    }

    return true;
}

#pragma region 

void UCreateSession::OnCreateSession(bool bWasSuccessful)
{

    if (bWasSuccessful)
    {
        if (MultiplayerSessionsSubsystem != nullptr)
            MultiplayerSessionsSubsystem->StartSession();
    }
    else
    {
        HostLobby->SetIsEnabled(true);
    }
}

void UCreateSession::OnHostLobbyButtonClicked()
{
    if (!MultiplayerSessionsSubsystem) return;

    HostLobby->SetIsEnabled(false);

    if (UdemyMultiplayerGameInstance != nullptr) {

        UdemyMultiplayerGameInstance->ShowLoadingScreen();

        ECheckBoxState CheckBoxState = this->IsLan->GetCheckedState();

        MultiplayerSessionsSubsystem->CreateSession(this->NumPublicConnections, this->MatchType, this->ServerNameTextBox->GetText(), ECheckBoxState::Checked == CheckBoxState ? true : false);
    }
}

void UCreateSession::OnStartSession(bool bWasSuccessful)
{
    if (bWasSuccessful)
    {
        if (UdemyMultiplayerGameInstance != nullptr) {

            ECheckBoxState CheckBoxState = this->IsLan->GetCheckedState();

            int32 playerCount = FCString::Atoi(*this->PlayersCountText->GetText().ToString());

            UdemyMultiplayerGameInstance->SetHostSettings(playerCount, ECheckBoxState::Checked == CheckBoxState ? true : false, this->ServerNameTextBox->GetText().ToString());

            UdemyMultiplayerGameInstance->GoToLobby();
        }               
    }
}

#pragma endregion CreateSessions

#pragma region

void UCreateSession::OnBackButtonClicked()
{    
    if (UdemyMultiplayerGameInstance != nullptr)
    {
        TearDown();
        UdemyMultiplayerGameInstance->LoadMainMenu();
    }    
}

#pragma endregion BackButton


void UCreateSession::OnDecrementPlayerCountButtonClick()
{
    int32 playerCount = FCString::Atoi(*this->PlayersCountText->GetText().ToString());
    
    playerCount--;

    if (playerCount >= 1)
    {
        this->PlayersCountText->SetText(FText::AsNumber(playerCount));
    }
}

void UCreateSession::OnIncrementPlayerCountButtonClick()
{
    int32 playerCount = FCString::Atoi(*this->PlayersCountText->GetText().ToString());

    playerCount++;

    if (playerCount <= this->NumPublicConnections)
    {
        this->PlayersCountText->SetText(FText::AsNumber(playerCount));
    }
}