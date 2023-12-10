// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionsMenu.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"

bool UOptionsMenu::Initialize()
{
    if (!Super::Initialize())
        return false;

    if (PreviousTextureQualityButton && NextTextureQualityButton &&
        PreviousResolutionButton && NextResolutionButton &&
        PreviousWindowModeButton && NextWindowModeButton &&
        ApplyButton && BackButton) {

        PreviousTextureQualityButton->OnClicked.AddDynamic(this, &UOptionsMenu::OnPreviousTextureQualityButtonClicked);
        NextTextureQualityButton->OnClicked.AddDynamic(this, &UOptionsMenu::OnNextTextureQualityButtonClicked);

        PreviousResolutionButton->OnClicked.AddDynamic(this, &UOptionsMenu::OnPreviousResolutionButtonClicked);
        NextResolutionButton->OnClicked.AddDynamic(this, &UOptionsMenu::OnNextResolutionButtonClicked);

        PreviousWindowModeButton->OnClicked.AddDynamic(this, &UOptionsMenu::OnPreviousWindowModeButtonClicked);
        NextWindowModeButton->OnClicked.AddDynamic(this, &UOptionsMenu::OnNextWindowModeButtonClicked);

        ApplyButton->OnClicked().AddUObject(this, &UOptionsMenu::OnApplyButtonClicked);
        BackButton->OnClicked().AddUObject(this, &UOptionsMenu::OnBackButtonClicked);
    }

    UWorld* World = GetWorld();

    if (IsValid(World))
    {
        UGameInstance* GameInstance = World->GetGameInstance();

        if (IsValid(GameInstance)) {
            UdemyMultiplayerGameInstance = Cast<UUdemyMultiplayerGameInstance>(GameInstance);
        }
    }

    this->InitializeSettings();
    this->InitializeScreenResolutionSupported();
    this->SetDefaultGameSettings();

    return true;
}


void UOptionsMenu::InitializeSettings()
{
    this->TextureQualityMaps.Add(0, FText::FromString("Low"));
    this->TextureQualityMaps.Add(1, FText::FromString("Medium"));
    this->TextureQualityMaps.Add(2, FText::FromString("High"));
    this->TextureQualityMaps.Add(3, FText::FromString("Epic"));

    this->WindowModeMaps.Add(EWindowMode::Fullscreen, FText::FromString("Fullscreen"));
    this->WindowModeMaps.Add(EWindowMode::Windowed, FText::FromString("Windowed"));
    this->WindowModeMaps.Add(EWindowMode::WindowedFullscreen, FText::FromString("Windowed Fullscreen"));

    this->WindowModeIndexMaps.Add(0, EWindowMode::Fullscreen);
    this->WindowModeIndexMaps.Add(1, EWindowMode::WindowedFullscreen);
    this->WindowModeIndexMaps.Add(2, EWindowMode::Windowed);
}

void UOptionsMenu::InitializeScreenResolutionSupported()
{
    UKismetSystemLibrary::GetSupportedFullscreenResolutions(this->SupportedResolutions);

    for (FIntPoint ResolutionItem : this->SupportedResolutions)
    {
        TArray<FStringFormatArg> ScreenResolutionFormats;
        ScreenResolutionFormats.Add(FStringFormatArg(ResolutionItem.X));
        ScreenResolutionFormats.Add(FStringFormatArg(ResolutionItem.Y));

        FString ScreenResolutionCurrentFormat = FString::Format(TEXT("{0}x{1}"), ScreenResolutionFormats);

        int32 ResolutionItemIndex = this->SupportedResolutions.IndexOfByKey(ResolutionItem);

        this->SupportedResolutionsMaps.Add(ResolutionItemIndex, FText::FromString(ScreenResolutionCurrentFormat));
    }
}

void UOptionsMenu::SetDefaultGameSettings()
{
    UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings();

    if (IsValid(GameUserSettings))
    {
        GameUserSettings->LoadSettings();

        this->SetTextureQuality(GameUserSettings->GetTextureQuality());
        this->SetScreenResolution(GameUserSettings->GetScreenResolution());
        this->SetWindowMode(GameUserSettings->GetFullscreenMode());
    }
}

void UOptionsMenu::SetTextureQuality(int32 InTextureQuality)
{
    this->TextureOption->SetText(this->TextureQualityMaps[InTextureQuality]);
}

void UOptionsMenu::SetScreenResolution(FIntPoint InScreenResolution)
{
    if (this->SupportedResolutions.Contains(InScreenResolution)) {

        TArray<FStringFormatArg> ScreenResolutionFormats;
        ScreenResolutionFormats.Add(FStringFormatArg(InScreenResolution.X));
        ScreenResolutionFormats.Add(FStringFormatArg(InScreenResolution.Y));

        FString ScreenResolutionCurrentFormat = FString::Format(TEXT("{0}x{1}"), ScreenResolutionFormats);

       this->ScreenResolutionOption->SetText(FText::FromString(ScreenResolutionCurrentFormat));
    }
}

void UOptionsMenu::SetWindowMode(EWindowMode::Type InWindowMode)
{
    this->WindowModeOption->SetText(this->WindowModeMaps[InWindowMode]);
}

void UOptionsMenu::OnApplyButtonClicked()
{
    UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings();

    if (IsValid(GameUserSettings))
    {
        GameUserSettings->LoadSettings();        

        GameUserSettings->SetTextureQuality(this->GetTextureIdByValueSelected());
        GameUserSettings->SetScreenResolution(this->GetScreenResolutionByValueSelected());
        GameUserSettings->SetFullscreenMode(this->GetWindowModeByValueSelected());

        GameUserSettings->ApplySettings(false);
    }
}

void UOptionsMenu::OnBackButtonClicked()
{
    if (this->UdemyMultiplayerGameInstance != nullptr) {
        this->UdemyMultiplayerGameInstance->SetBackToMainMenu(true);
        this->UdemyMultiplayerGameInstance->SetOptionsMenu(false);
        this->UdemyMultiplayerGameInstance->OpenNextLevel("MainMenu", false, false, 0.1f);
    }
}

void UOptionsMenu::OnPreviousTextureQualityButtonClicked()
{
    FText InTextureQualityName = TextureOption->GetText();
    int32 TextureKey = 2;

    for (TPair<int32, FText> TextureQualityItem : this->TextureQualityMaps)
    {
        if (TextureQualityItem.Value.EqualTo(InTextureQualityName)) {

            if ((TextureQualityItem.Key - 1) <= 0)
                TextureKey = 0;
            else
                TextureKey = TextureQualityItem.Key - 1;
        }
    }
    
    this->SetTextureQuality(TextureKey);
}

void UOptionsMenu::OnNextTextureQualityButtonClicked()
{
    FText InTextureQualityName = TextureOption->GetText();
    int32 TextureKey = 2;

    for (TPair<int32, FText> TextureQualityItem : this->TextureQualityMaps)
    {
        if (TextureQualityItem.Value.EqualTo(InTextureQualityName)) {

            if ((TextureQualityItem.Key + 1) >= 3)
                TextureKey = 3;
            else
                TextureKey = TextureQualityItem.Key + 1;
        }
    }

    this->SetTextureQuality(TextureKey);
}

void UOptionsMenu::OnPreviousResolutionButtonClicked()
{
    FText InScrenResolutionName = ScreenResolutionOption->GetText();
    int32 ScreenResolutionKey = 0;

    for (TPair<int32, FText> ScreenResolutionItem : this->SupportedResolutionsMaps)
    {
        if (ScreenResolutionItem.Value.EqualTo(InScrenResolutionName)) {
            if ((ScreenResolutionItem.Key - 1) <= 0)
                ScreenResolutionKey = 0;
            else
                ScreenResolutionKey = ScreenResolutionItem.Key - 1;
        }
    }

    this->ScreenResolutionOption->SetText(this->SupportedResolutionsMaps[ScreenResolutionKey]);
}

void UOptionsMenu::OnNextResolutionButtonClicked()
{
    FText InScrenResolutionName = ScreenResolutionOption->GetText();
    int32 ScreenResolutionKey = 0;

    for (TPair<int32, FText> ScreenResolutionItem : this->SupportedResolutionsMaps)
    {
        if (ScreenResolutionItem.Value.EqualTo(InScrenResolutionName)) {

            if ((ScreenResolutionItem.Key + 1) >= (this->SupportedResolutionsMaps.Num()))
                ScreenResolutionKey = (this->SupportedResolutionsMaps.Num() - 1);
            else
                ScreenResolutionKey = ScreenResolutionItem.Key + 1;
        }
    }

    this->ScreenResolutionOption->SetText(this->SupportedResolutionsMaps[ScreenResolutionKey]);
}

void UOptionsMenu::OnPreviousWindowModeButtonClicked()
{
    FText InWindowModeName = WindowModeOption->GetText();
    int32 WindowModeKey = 0;

    for (TPair<int32, FText> WindowModeItem : this->WindowModeMaps)
    {
        if (WindowModeItem.Value.EqualTo(InWindowModeName)) {

            if ((WindowModeItem.Key - 1) <= 0)
                WindowModeKey = 0;
            else
                WindowModeKey = WindowModeItem.Key - 1;
        }
    }

    this->SetWindowMode(*this->WindowModeIndexMaps.Find(WindowModeKey));
}

void UOptionsMenu::OnNextWindowModeButtonClicked()
{
    FText InWindowModeName = WindowModeOption->GetText();
    int32 WindowModeKey = 0;

    for (TPair<EWindowMode::Type, FText> WindowModeItem : this->WindowModeMaps)
    {
        if (WindowModeItem.Value.EqualTo(InWindowModeName)) {

            if ((WindowModeItem.Key + 1) >= 2)
                WindowModeKey = 2;
            else
                WindowModeKey = WindowModeItem.Key + 1;
        }
    }

    this->SetWindowMode(*this->WindowModeIndexMaps.Find(WindowModeKey));
}

int32 UOptionsMenu::GetTextureIdByValueSelected()
{
    FText InTextureQualityName = TextureOption->GetText();

    int32 TextureQualityId = 0;

    for (TPair<int32, FText> TextureQualityItem : this->TextureQualityMaps)
    {
        if (TextureQualityItem.Value.EqualTo(InTextureQualityName)) {
            TextureQualityId = TextureQualityItem.Key;
        }
    }

    return TextureQualityId;
}

FIntPoint UOptionsMenu::GetScreenResolutionByValueSelected()
{
    auto InScrenResolutionName = ScreenResolutionOption->GetText().ToString();

    TArray<FString> Substrings;
    FString Delimiter = "x";

    InScrenResolutionName.ParseIntoArray(Substrings, *Delimiter, true);

    FIntPoint IntPoint;
    IntPoint.X = FCString::Atoi(*Substrings[0]);
    IntPoint.Y = FCString::Atoi(*Substrings[1]);

    return IntPoint;
}

EWindowMode::Type UOptionsMenu::GetWindowModeByValueSelected()
{
    FText InWindowModeName = WindowModeOption->GetText();
    EWindowMode::Type WindowMode = EWindowMode::Fullscreen;

    for (TPair<EWindowMode::Type, FText> WindowModeItem : this->WindowModeMaps)
    {
        if (WindowModeItem.Value.EqualTo(InWindowModeName))
            WindowMode = WindowModeItem.Key;        
    }

    return WindowMode;
}
