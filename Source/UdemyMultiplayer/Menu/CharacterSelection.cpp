// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelection.h"
#include "../LobbyPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Animation/UMGSequencePlayer.h"


bool UCharacterSelection::Initialize() {
    
    if (!Super::Initialize())
        return false;

    if (WarriorButton && ArcherButton && WizardButton) {

        WarriorButton->OnClicked.AddDynamic(this, &UCharacterSelection::OnWarriorButtonClicked);
        ArcherButton->OnClicked.AddDynamic(this, &UCharacterSelection::OnArcherButtonClicked);
        WizardButton->OnClicked.AddDynamic(this, &UCharacterSelection::OnWizardButtonClicked);

        StartDelegate.BindDynamic(this, &UCharacterSelection::AnimationStarted);
        EndDelegate.BindDynamic(this, &UCharacterSelection::AnimationFinished);

        BindToAnimationStarted(this->FadeMenu, StartDelegate);
        BindToAnimationFinished(this->FadeMenu, EndDelegate);
    }

    this->CurrentCharacterSelected = 0;
    
    this->SetRenderOpacity(0);
    this->SetVisibility(ESlateVisibility::HitTestInvisible);

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

void UCharacterSelection::ToggleMenu()
{    
    //if opacity is 0 then the menu its hidden.
    if (0.0f == this->GetRenderOpacity())
        this->PlayAnimationForward(this->FadeMenu, 4);
    else
        this->PlayAnimationReverse(this->FadeMenu);
    
}

void UCharacterSelection::AnimationStarted()
{
    this->CurrentOpacity = this->GetRenderOpacity();
}

void UCharacterSelection::AnimationFinished()
{
    if (this->CurrentOpacity == 0.0f) {
        this->SetVisibility(ESlateVisibility::Visible);
        this->SetRenderOpacity(1);
    }        
    else {
        this->SetRenderOpacity(0);
        this->SetVisibility(ESlateVisibility::HitTestInvisible);
    }
        
}

void UCharacterSelection::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UCharacterSelection, CurrentCharacterSelected);
}