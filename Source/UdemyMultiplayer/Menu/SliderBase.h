// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Components/Slider.h"
#include "CommonTextBlock.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"
#include "SliderBase.generated.h"


/**
 *
 */
UCLASS()
class UDEMYMULTIPLAYER_API USliderBase : public UUserWidget
{
    GENERATED_BODY()

    virtual bool Initialize();

public:

    UPROPERTY(EditAnywhere, Category = "Sound Settings")
        FString Title;

    UPROPERTY(EditAnywhere, Category = "Sound Settings")
        USoundClass* SoundClass;

    UPROPERTY(EditAnywhere, Category = "Sound Settings")
        USoundMix* SoundMix;

    UPROPERTY(meta = (BindWidget))
        UCommonTextBlock* Description;

    UPROPERTY(meta = (BindWidget))
        USlider* VolumeRange;

public:

    UFUNCTION() void OnVolumeValueChanged(float Value);    
    UFUNCTION(BlueprintCallable) void SetTitle(FText InTitle);
    UFUNCTION(BlueprintCallable) void GetTitle();
};
