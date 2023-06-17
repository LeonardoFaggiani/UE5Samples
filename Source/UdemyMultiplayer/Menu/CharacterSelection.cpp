// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelection.h"
#include "../LobbyPlayerController.h"
#include "Net/UnrealNetwork.h"

bool UCharacterSelection::Initialize() {
    
    if (!Super::Initialize())
        return false;

    if (WarriorButton && ArcherButton && WizardButton) {

        WarriorButton->OnClicked.AddDynamic(this, &UCharacterSelection::OnWarriorButtonClicked);
        ArcherButton->OnClicked.AddDynamic(this, &UCharacterSelection::OnArcherButtonClicked);
        WizardButton->OnClicked.AddDynamic(this, &UCharacterSelection::OnWizardButtonClicked);
    }

    this->CurrentCharacterSelected = 0;

    return true;
}


void UCharacterSelection::OnWarriorButtonClicked()
{
    this->CurrentCharacterSelected = 0;
    this->NotifyPlayerController();
}

void UCharacterSelection::OnArcherButtonClicked()
{
    this->CurrentCharacterSelected = 1;
    this->NotifyPlayerController();
}

void UCharacterSelection::OnWizardButtonClicked()
{
    this->CurrentCharacterSelected = 2;
    this->NotifyPlayerController();
}

void UCharacterSelection::NotifyPlayerController()
{
    ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(this->GetOwningPlayer());

    LobbyPlayerController->Client_AssignPlayer(this->CurrentCharacterSelected);

}

void UCharacterSelection::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UCharacterSelection, CurrentCharacterSelected);
}