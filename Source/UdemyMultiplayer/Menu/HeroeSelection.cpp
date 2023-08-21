// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroeSelection.h"
#include "../LobbyPlayerController.h"
#include "Components/HorizontalBoxSlot.h"
#include "HeroeItem.h"

bool UHeroeSelection::Initialize() {
    
    if (!Super::Initialize())
        return false;

    UWorld* World = GetWorld();

    if (IsValid(World))
    {
        UGameInstance* GameInstance = World->GetGameInstance();

        if (IsValid(GameInstance)) {
            UdemyMultiplayerGameInstance = Cast<UUdemyMultiplayerGameInstance>(GameInstance);
        }
    }

    if (BackButton) 
        BackButton->OnClicked().AddUObject(this, &ThisClass::OnBackButtonClicked);

    this->FillContainer();

    return true;
}

void UHeroeSelection::OnBackButtonClicked()
{    
    RemoveFromParent();

    ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(this->GetOwningPlayer());

    LobbyPlayerController->Client_SwitchToLobbyMode();
}

void UHeroeSelection::FillContainer()
{
    //Fill the container of heroes with the card of each heroes and thier stats
    this->Container->ClearChildren();

    if (IsValid(this->UdemyMultiplayerGameInstance))
    {
        for (FHeroeResources HeroeResources : this->UdemyMultiplayerGameInstance->HeroeResources)
        {
            UHeroeItem* InHeroeItem = CreateWidget<UHeroeItem>(this, this->HeroeItem);
            
            InHeroeItem->SetPadding(FMargin{ 8,0,8,5 });
            InHeroeItem->SetHeroeName(HeroeResources.Name);
            InHeroeItem->SetHeroeIcon(HeroeResources.HeroeIcon);
            InHeroeItem->SetAttributeStats(HeroeResources);

            UHorizontalBoxSlot* HorizontalBoxSlot = this->Container->AddChildToHorizontalBox(InHeroeItem);

            HorizontalBoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Bottom);
        }
    }
}