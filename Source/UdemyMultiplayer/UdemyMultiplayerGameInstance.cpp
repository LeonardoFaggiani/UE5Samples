// Fill out your copyright notice in the Description page of Project Settings.


#include "UdemyMultiplayerGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "UdemyMultiplayerCharacter.h"
#include "Animation/UMGSequencePlayer.h"
#include "Engine/EngineTypes.h"
#include "TimerManager.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Slate.h"
#include "MoviePlayer.h"



UUdemyMultiplayerGameInstance::UUdemyMultiplayerGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    static ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/UI/Menu/WBP_Menu"));
    static ConstructorHelpers::FClassFinder<UUserWidget> LoadingScreenBPClass(TEXT("/Game/UI/Menu/WBP_LoadingScreen"));

    static ConstructorHelpers::FClassFinder<AUdemyMultiplayerCharacter> WarriorBPClass(TEXT("/Game/Blueprints/Characters/BP_Warrior"));
    static ConstructorHelpers::FClassFinder<AUdemyMultiplayerCharacter> ArcherBPClass(TEXT("/Game/Blueprints/Characters/BP_Archer"));
    static ConstructorHelpers::FClassFinder<AUdemyMultiplayerCharacter> WizardBPClass(TEXT("/Game/Blueprints/Characters/BP_Wizard"));

    if (!ensure(WarriorBPClass.Class != nullptr)) return;
    if (!ensure(ArcherBPClass.Class != nullptr)) return;
    if (!ensure(WizardBPClass.Class != nullptr)) return;

    this->Characters.Add(WarriorBPClass.Class);
    this->Characters.Add(ArcherBPClass.Class);
    this->Characters.Add(WizardBPClass.Class);

    if (!ensure(MenuBPClass.Class != nullptr)) return;
    if (!ensure(LoadingScreenBPClass.Class != nullptr)) return;

    MenuClass = MenuBPClass.Class;
    LoadingScreenClass = LoadingScreenBPClass.Class;

    this->InitializePlayerSpot();
}

void UUdemyMultiplayerGameInstance::Init()
{
    Super::Init();

    FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UUdemyMultiplayerGameInstance::BeginLoadingScreen);
    FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UUdemyMultiplayerGameInstance::EndLoadingScreen);    
    
    if (!ensure(LoadingScreenClass != nullptr)) return;

    this->LoadingScreen = CreateWidget<ULoadingScreen>(this, LoadingScreenClass);
}

UMainMenu* UUdemyMultiplayerGameInstance::LoadMenu()
{
    this->InitializeMapConfigurations();

    if (!ensure(MenuClass != nullptr)) return nullptr;
    Menu = CreateWidget<UMainMenu>(this, MenuClass);

    if (!ensure(Menu != nullptr)) return nullptr;

    Menu->Setup();
    Menu->SetMenuInterface(this);
    MultiplayerSessionsSubsystem = this->GetSubsystem<UMultiplayerSessionsSubsystem>();

    return Menu;
}

void UUdemyMultiplayerGameInstance::BeginLoadingScreen_Implementation(const FString& InMapName)
{
    if (InMapName == "/Game/ThirdPerson/Maps/LobbyChampionSelection")
    {
        UUserWidget* UserWidget = CreateWidget<UUserWidget>(this, LoadingScreenWidget);

        FLoadingScreenAttributes LoadingScreenAttributes;
        LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = 1;
        LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = false;
        LoadingScreenAttributes.bMoviesAreSkippable = false;
        LoadingScreenAttributes.bWaitForManualStop = true;
        LoadingScreenAttributes.bAllowEngineTick = true;
        LoadingScreenAttributes.WidgetLoadingScreen = this->LoadingScreen->TakeWidget();

        GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);
    }
}

void UUdemyMultiplayerGameInstance::EndLoadingScreen_Implementation(UWorld* InLoadedWorld)
{
    if (InLoadedWorld->GetName() == "LobbyChampionSelection") {
        
    }
}

void UUdemyMultiplayerGameInstance::Join()
{
    MultiplayerSessionsSubsystem->FindSessions(50000, false);
}

void UUdemyMultiplayerGameInstance::Quit()
{
    UWorld* World = GetWorld();

    if (!ensure(World != nullptr)) return;

    APlayerController* PlayerController = World->GetFirstPlayerController();

    if (!ensure(PlayerController != nullptr)) return;

    PlayerController->ConsoleCommand("quit");
}

void UUdemyMultiplayerGameInstance::LoadMainMenu()
{
    APlayerController* PlayerController = GetFirstLocalPlayerController();

    if (!ensure(PlayerController != nullptr)) return;

    PlayerController->ClientTravel("/Game/ThirdPerson/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
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
    if (!ensure(this->LoadingScreen != nullptr)) return;

    if (!this->LoadingScreen->IsInViewport())
        this->LoadingScreen->Setup();   

    bWithTransition ? this->LoadingScreen->TransBounceIn() : this->LoadingScreen->TransBounceInCompleted();

    this->LoadingScreen->SetMenuInterface(this);
}

void UUdemyMultiplayerGameInstance::HideLoadingScreen(bool bWithTransition)
{
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

void UUdemyMultiplayerGameInstance::StopMovie()
{    
    GetMoviePlayer()->StopMovie();   
    //HideLoadingScreen();
}

void UUdemyMultiplayerGameInstance::OpenLevelWithDelay(FName InLevelName, FString InListen)
{
    UGameplayStatics::OpenLevel(GWorld, InLevelName, true, InListen);
}

void UUdemyMultiplayerGameInstance::InitializeMapConfigurations()
{
    this->ConfigurationMaps.Add("LobbyMap", FConfigurationMaps{ TEXT("Game/ThirdPerson/Maps/ThirdPersonMap?listen"), TEXT("/Game/UI/Menu/Textures/LobbyMap"),TEXT("ThirdPersonMap"), 1 });
    this->ConfigurationMaps.Add("CementeryMap", FConfigurationMaps{ TEXT("/Game/ThirdPerson/Maps/Cementery?listen"), TEXT("/Game/UI/Menu/Textures/CementeryMap"),TEXT("Cementery"), 2 });
    this->ConfigurationMaps.Add("EgyptMap", FConfigurationMaps{ TEXT("/Game/ThirdPerson/Maps/Egypt?listen"), TEXT("/Game/UI/Menu/Textures/EgyptMap"),TEXT("Egypt"), 3 });
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
    DOREPLIFETIME(UUdemyMultiplayerGameInstance, Characters);
}
