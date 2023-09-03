// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroeItem.h"
#include "../LobbyPlayerController.h"
#include "Components/HorizontalBoxSlot.h"
#include <Kismet/GameplayStatics.h>
#include <UdemyMultiplayer/UdemyMultiplayerPlayerState.h>

UHeroeItem::UHeroeItem()
{
    this->OnClicked().AddUObject(this, &ThisClass::SetHeroeSelected);
}


void UHeroeItem::SetHeroeSelected()
{
    ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(this->GetOwningPlayer());

    if (IsValid(LobbyPlayerController)) {

        RemoveDisabledStateToAllItems();

        UWorld* World = GetWorld();

        if (IsValid(World))
        {
            UGameInstance* GameInstance = World->GetGameInstance();

            if (IsValid(GameInstance))
            {
                this->SetIsEnabled(false);

                UUdemyMultiplayerGameInstance* UdemyMultiplayerGameInstance = Cast<UUdemyMultiplayerGameInstance>(GameInstance);

                TSubclassOf<AUdemyMultiplayerCharacter> UdemyMultiplayerCharacter = UdemyMultiplayerGameInstance->GetHeroeByName(this->HeroeName->GetText().ToString());

                LobbyPlayerController->Client_AssignHeroeToPlayer(UdemyMultiplayerCharacter);
            }
        }
    }
}

void UHeroeItem::SetAttributeStats(const FHeroeResources& InHeroeResources)
{
    //For each heroe set the status
    for (const FHeroeAttribute& HeroeAttribute : InHeroeResources.HeroeAttributes)
    {
        UHeroeAttributeStats* InHeroeAttributeStats = CreateWidget<UHeroeAttributeStats>(this, this->HeroeAttributeStats);

        InHeroeAttributeStats->SetIcon(HeroeAttribute.Icon);
        InHeroeAttributeStats->SetPoints(HeroeAttribute.Points);

        UHorizontalBoxSlot* HorizontalBoxSlot = this->StatsContainer->AddChildToHorizontalBox(InHeroeAttributeStats);

        FSlateChildSize SlateChildSize;

        HorizontalBoxSlot->SetSize(SlateChildSize);        
    }
}

void UHeroeItem::SetHeroeName(FString InHeroeName)
{
    this->HeroeName->SetText(FText::FromString(InHeroeName));
}

void UHeroeItem::SetHeroeIcon(FString InHeroeIcon)
{
    UTexture2D* IconTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *InHeroeIcon));

    if (IsValid(IconTexture))
        this->HeroeIcon->SetBrushFromTexture(IconTexture);
}

void UHeroeItem::RemoveDisabledStateToAllItems()
{
    ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(this->GetOwningPlayer());

    AUdemyMultiplayerPlayerState* UdemyMultiplayerPlayerState = LobbyPlayerController->GetPlayerState<AUdemyMultiplayerPlayerState>();

    for (UHeroeItem* HeroeItem : UdemyMultiplayerPlayerState->GetHeroesItems())
        HeroeItem->SetIsEnabled(true);
}