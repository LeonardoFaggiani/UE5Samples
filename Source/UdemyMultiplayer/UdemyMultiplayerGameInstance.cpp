// Fill out your copyright notice in the Description page of Project Settings.


#include "UdemyMultiplayerGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/UMGSequencePlayer.h"
#include "Engine/EngineTypes.h"
#include "TimerManager.h"
#include "Runtime/UMG/Public/UMG.h"
#include "UdemyMultiplayerCharacter.h"
#include "Slate.h"
#include "Menu/Enum/AttributeType.h"
#include "Menu/Struct/HeroeAttribute.h"


UUdemyMultiplayerGameInstance::UUdemyMultiplayerGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    this->InitializeHeroes();
    this->InitializePlayerSpot();
}

void UUdemyMultiplayerGameInstance::Init()
{
    Super::Init();

    FWorldDelegates::LevelRemovedFromWorld.AddUObject(this, &ThisClass::OnLevelRemovedFromWorld);
    FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UUdemyMultiplayerGameInstance::BeginLoadingScreen);
    FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UUdemyMultiplayerGameInstance::EndLoadingScreen);

    if (!ensure(this->LoadingScreenClass != nullptr)) return;

    this->LoadingScreen = CreateWidget<ULoadingScreen>(this, LoadingScreenClass);

    this->LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = 5;
    this->LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = true;
    this->LoadingScreenAttributes.bMoviesAreSkippable = false;
    this->LoadingScreenAttributes.bWaitForManualStop = true;
    this->LoadingScreenAttributes.bAllowEngineTick = true;        
    this->LoadingScreenAttributes.WidgetLoadingScreen = this->LoadingScreen->TakeWidget();
}

UMainMenu* UUdemyMultiplayerGameInstance::LoadMenu()
{
    if (!ensure(MenuClass != nullptr)) return nullptr;
    Menu = CreateWidget<UMainMenu>(this, MenuClass);

    if (!ensure(Menu != nullptr)) return nullptr;

    Menu->Setup();
    Menu->SetMenuInterface(this);

    return Menu;
}

void UUdemyMultiplayerGameInstance::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) {

        if (InLevel == nullptr) {
            if (InWorld->GetFName() == FName("MainMenuWidgets"))
            {
                if (this->LoadingScreen->IsInViewport()) {

                    this->LoadingScreen->TransBounceInCompleted();

                    this->LoadingScreen->SetMenuInterface(this);
                }
            }
        }
    
}
void UUdemyMultiplayerGameInstance::EndLoadingScreen_Implementation(UWorld* InLoadedWorld)
{
    if (InLoadedWorld->GetName() == "LobbyChampionSelection")
    {
        if (!this->LoadingScreen->IsInViewport()) {
            this->ShowLoadingScreen(false);
            this->LoadingScreen->SetMenuInterface(this);
        }
    }
}

void UUdemyMultiplayerGameInstance::BeginLoadingScreen_Implementation(const FString& InMapName)
{
    if (InMapName == "/Game/ThirdPerson/Maps/LobbyChampionSelection")
    {
        if (this->LoadingScreen->IsInViewport()) {

            this->LoadingScreen->TransBounceInCompleted();

            this->LoadingScreen->SetMenuInterface(this);
        }
    }
}

void UUdemyMultiplayerGameInstance::InitializeHeroes() {

    static ConstructorHelpers::FClassFinder<AUdemyMultiplayerCharacter> WarriorBPClass(TEXT("/Game/Blueprints/Characters/BP_Warrior"));
    static ConstructorHelpers::FClassFinder<AUdemyMultiplayerCharacter> ArcherBPClass(TEXT("/Game/Blueprints/Characters/BP_Archer"));
    static ConstructorHelpers::FClassFinder<AUdemyMultiplayerCharacter> WizardBPClass(TEXT("/Game/Blueprints/Characters/BP_Wizard"));

    if (!ensure(WarriorBPClass.Class != nullptr)) return;
    if (!ensure(ArcherBPClass.Class != nullptr)) return;
    if (!ensure(WizardBPClass.Class != nullptr)) return;

    TArray<FHeroeAttribute> WarriorAttributes;
    TArray<FHeroeAttribute> ArcherAttributes;
    TArray<FHeroeAttribute> WizardAttributes;

    WarriorAttributes.Add(FHeroeAttribute{ TEXT("/Game/UI/Materials/Textures/sword-icon"), EAttributeType::Attack, 25 });
    WarriorAttributes.Add(FHeroeAttribute{ TEXT("/Game/UI/Materials/Textures/shield-icon"), EAttributeType::Armor, 60 });
    WarriorAttributes.Add(FHeroeAttribute{ TEXT("/Game/UI/Materials/Textures/healtpoints-icon"), EAttributeType::Health, 600 });
    
    ArcherAttributes.Add(FHeroeAttribute{ TEXT("/Game/UI/Materials/Textures/bow-icon"), EAttributeType::Attack, 45 });
    ArcherAttributes.Add(FHeroeAttribute{ TEXT("/Game/UI/Materials/Textures/shield-icon"), EAttributeType::Armor, 30 });
    ArcherAttributes.Add(FHeroeAttribute{ TEXT("/Game/UI/Materials/Textures/healtpoints-icon"), EAttributeType::Health, 300 });

    WizardAttributes.Add(FHeroeAttribute{ TEXT("/Game/UI/Materials/Textures/magic-staff-icon"), EAttributeType::Attack, 60 });
    WizardAttributes.Add(FHeroeAttribute{ TEXT("/Game/UI/Materials/Textures/shield-icon"), EAttributeType::Armor, 20 });
    WizardAttributes.Add(FHeroeAttribute{ TEXT("/Game/UI/Materials/Textures/healtpoints-icon"), EAttributeType::Health, 200 });

    this->HeroeResources.Add(FHeroeResources{ "Warrior", WarriorBPClass.Class, TEXT("/Game/UI/Materials/Textures/Warrior"), WarriorAttributes });
    this->HeroeResources.Add(FHeroeResources{ "Archer", ArcherBPClass.Class, TEXT("/Game/UI/Materials/Textures/Archer"), ArcherAttributes });
    this->HeroeResources.Add(FHeroeResources{ "Wizard", WizardBPClass.Class, TEXT("/Game/UI/Materials/Textures/Wizard"), WizardAttributes });
}

void UUdemyMultiplayerGameInstance::Quit()
{
    UWorld* World = GetWorld();

    if (!ensure(World != nullptr)) return;

    APlayerController* PlayerController = World->GetFirstPlayerController();

    if (!ensure(PlayerController != nullptr)) return;

    PlayerController->ConsoleCommand("quit");
}

void UUdemyMultiplayerGameInstance::OpenNextLevel(FName InLevel, bool bIsListen, bool bShowLoading, float OpenLevelDelay)
{
    UWorld* World = GetWorld();

    if (World) {
        
        if (bShowLoading)
            this->ShowLoadingScreen(true);

        FTimerHandle MemberTimerHandle;
        FTimerDelegate TimerDel;

        FString ListenLevel = bIsListen ? "listen" : "";

        TimerDel.BindUFunction(this, FName("OpenLevelWithDelay"), InLevel, ListenLevel);

        GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, TimerDel, OpenLevelDelay, false);
    }
}

void UUdemyMultiplayerGameInstance::SetHostSettings(int32 NumberOfPlayers, FString serverName) {
    this->MaxPlayers = NumberOfPlayers;
    this->ServerName = serverName;
}

void UUdemyMultiplayerGameInstance::ShowLoadingScreen(bool bWithTransition)
{
    UE_LOG(LogTemp, Warning, TEXT("ShowLoadingScreen OK!"));
    
    if (!ensure(this->LoadingScreen != nullptr)) return;

    if (!this->LoadingScreen->IsInViewport()) {
        this->LoadingScreen->Setup(1);
    }

    bWithTransition ? this->LoadingScreen->TransBounceIn() : this->LoadingScreen->TransBounceInCompleted();

    this->LoadingScreen->SetMenuInterface(this);
}

void UUdemyMultiplayerGameInstance::HideLoadingScreen(bool bWithTransition)
{
    UE_LOG(LogTemp, Warning, TEXT("HideLoadingScreen OK!"));

    if (!ensure(this->LoadingScreen != nullptr)) return;

    if (!this->LoadingScreen->IsInViewport())
        this->LoadingScreen->Setup();

    this->LoadingScreen->TransBounceOut();
    this->LoadingScreen->SetMenuInterface(this);
}

void UUdemyMultiplayerGameInstance::SetHostGame(bool InbIsHostGameMenu)
{
    this->bIsHostGameMenu = InbIsHostGameMenu;
}

void UUdemyMultiplayerGameInstance::SetFindGames(bool InbIsFindGamesMenu)
{
    this->bIsFindGamesMenu = InbIsFindGamesMenu;
}

void UUdemyMultiplayerGameInstance::SetBackToMainMenu(bool InbIsBackToMainMenu)
{   
    this->bIsBackToMainMenu = InbIsBackToMainMenu;
}

void UUdemyMultiplayerGameInstance::SetOptionsMenu(bool InbIsOptionsMenu)
{
    this->bIsOptionsMenu = InbIsOptionsMenu;
}

void UUdemyMultiplayerGameInstance::SetFirstTimeLoading(bool InbIsFirstTimeLoading)
{
    this->bIsFirstTimeLoading = InbIsFirstTimeLoading;
}

bool UUdemyMultiplayerGameInstance::GetBackToMainMenu()
{
    return this->bIsBackToMainMenu;
}

bool UUdemyMultiplayerGameInstance::GetFindGames()
{
    return this->bIsFindGamesMenu;
}

bool UUdemyMultiplayerGameInstance::GetHostGame()
{
    return this->bIsHostGameMenu;
}

bool UUdemyMultiplayerGameInstance::GetOptionsMenu()
{
    return this->bIsOptionsMenu;
}

bool UUdemyMultiplayerGameInstance::GetIsFirstTimeLoading()
{
    return this->bIsFirstTimeLoading;
}

void UUdemyMultiplayerGameInstance::PlayEnviromentMusic(USoundBase* Audio, float Volume, bool bIsPersistLevel)
{
    if (!ensure(Audio != nullptr)) return;

    this->Music = UGameplayStatics::CreateSound2D(GetWorld(), Audio, Volume, 1, 0, nullptr, bIsPersistLevel, true);

    this->Music->Play();
}


void UUdemyMultiplayerGameInstance::StopMovie()
{    

    GetMoviePlayer()->StopMovie();   
}

TSubclassOf<AUdemyMultiplayerCharacter> UUdemyMultiplayerGameInstance::GetHeroeByName(FString InHeroeName)
{
    auto GetHeroeByName = [InHeroeName](const FHeroeResources& Heroe) {
        return Heroe.Name == InHeroeName;
    };

    auto InHeroeResource = this->HeroeResources.FindByPredicate(GetHeroeByName);

    return InHeroeResource->TargetClass;
}

void UUdemyMultiplayerGameInstance::OpenLevelWithDelay(FName InLevelName, FString InListen)
{
    UGameplayStatics::OpenLevel(GWorld, InLevelName, true, InListen);
}

void UUdemyMultiplayerGameInstance::InitializePlayerSpot()
{
    this->ConfigurationPlayerSpot.Add(1, FPlayerSpot{ 1, FVector(790,230,0), FRotator(0,90,0) });
    this->ConfigurationPlayerSpot.Add(2, FPlayerSpot{ 2, FVector(890,190,0), FRotator(0,90,0) });
    this->ConfigurationPlayerSpot.Add(3, FPlayerSpot{ 3, FVector(1000,190,0), FRotator(0,90,0) });
    this->ConfigurationPlayerSpot.Add(4, FPlayerSpot{ 4, FVector(1100,230,0), FRotator(0,90,0) });
}

void UUdemyMultiplayerGameInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UUdemyMultiplayerGameInstance, MaxPlayers);
    DOREPLIFETIME(UUdemyMultiplayerGameInstance, ServerName);
}