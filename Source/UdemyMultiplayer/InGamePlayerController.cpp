// Fill out your copyright notice in the Description page of Project Settings.


#include "InGamePlayerController.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputTriggers.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void AInGamePlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(this->CommonPlayerControllerMappingContext, 0);
    }

    FInputModeGameAndUI FInputModeGameAndUI;

    this->SetInputMode(FInputModeGameAndUI);
}

void AInGamePlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
        EnhancedInputComponent->BindAction(this->ShowInGameMenuAction, ETriggerEvent::Triggered, this, &ThisClass::ShowInGameMenu);
    }
}

void AInGamePlayerController::SetCurrentCharacter(AUdemyMultiplayerCharacter* InCurrentCharacter)
{
    this->CurrentCharacter = InCurrentCharacter;
}

AUdemyMultiplayerCharacter* AInGamePlayerController::GetCurrentCharacter()
{
    return this->CurrentCharacter;
}

void AInGamePlayerController::ShowInGameMenu(const FInputActionValue& Value)
{
    if (!ensure(this->InGameMenuClass != nullptr)) return;

    this->InGameMenu = CreateWidget<UInGameMenu>(this, InGameMenuClass);
    this->InGameMenu->Setup();
}