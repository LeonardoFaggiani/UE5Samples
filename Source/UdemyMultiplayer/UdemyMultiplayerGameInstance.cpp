// Fill out your copyright notice in the Description page of Project Settings.


#include "UdemyMultiplayerGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Menu/CreateSession.h"
#include "Components/WidgetSwitcher.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

UUdemyMultiplayerGameInstance::UUdemyMultiplayerGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/UI/Menu/WBP_Menu"));
	static ConstructorHelpers::FClassFinder<UUserWidget> LoadingScreenBPClass(TEXT("/Game/UI/Menu/WBP_LoadingScreen"));

	if (!ensure(MenuBPClass.Class != nullptr)) return;
	if (!ensure(LoadingScreenBPClass.Class != nullptr)) return;

	MenuClass = MenuBPClass.Class;
	LoadingScreenClass = LoadingScreenBPClass.Class;

	this->InitializePlayerSpot();
}

void UUdemyMultiplayerGameInstance::LoadMenu()
{	
	this->InitializeMapConfigurations();

	if (!ensure(MenuClass != nullptr)) return;
	Menu = CreateWidget<UMainMenu>(this, MenuClass);

	if (!ensure(Menu != nullptr)) return;

	Menu->Setup();
	Menu->SetMenuInterface(this);
	MultiplayerSessionsSubsystem = this->GetSubsystem<UMultiplayerSessionsSubsystem>();
}

void UUdemyMultiplayerGameInstance::Host()
{
	SetSwitchByIndex(1);	
}

void UUdemyMultiplayerGameInstance::Join(bool bIsLan)
{
	if (!MultiplayerSessionsSubsystem) return;

	SetSwitchByIndex(2);

	MultiplayerSessionsSubsystem->FindSessions(10000, bIsLan);
}

void UUdemyMultiplayerGameInstance::GoToLobby()
{
	UWorld* World = GetWorld();

	if (World)	
		World->ServerTravel(TEXT("/Game/ThirdPerson/Maps/Lobby?listen"));
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

void UUdemyMultiplayerGameInstance::SetSwitchByIndex(int32 Index) {

	UWidgetSwitcher* WidgetSwitcher = Menu->GetMenuSwitcher();

	WidgetSwitcher->SetActiveWidgetIndex(Index);	
}

void UUdemyMultiplayerGameInstance::SetHostSettings(int32 NumberOfPlayers, bool bIsLan, FString serverName) {
    this->MaxPlayers = NumberOfPlayers;
    this->bIsLAN = bIsLan;
    this->ServerName = serverName;
}

void UUdemyMultiplayerGameInstance::ShowLoadingScreen() {

	if (!ensure(LoadingScreenClass != nullptr)) return;
	LoadingScreen = CreateWidget<ULoadingScreen>(this, LoadingScreenClass);

	if (!ensure(LoadingScreen != nullptr)) return;

	LoadingScreen->Setup();
	LoadingScreen->SetMenuInterface(this);
}

void UUdemyMultiplayerGameInstance::InitializeMapConfigurations()
{	
    this->ConfigurationMaps.Add("LobbyMap", FConfigurationMaps{ TEXT("/Game/ThirdPerson/Maps/Lobby?listen"), TEXT("/Game/UI/Menu/Textures/LobbyMap"),TEXT("Lobby"), 1 });
	this->ConfigurationMaps.Add("CementeryMap", FConfigurationMaps{ TEXT("/Game/ThirdPerson/Maps/Cementery?listen"), TEXT("/Game/UI/Menu/Textures/CementeryMap"),TEXT("Cementery"), 2 });
	this->ConfigurationMaps.Add("EgyptMap", FConfigurationMaps{ TEXT("/Game/ThirdPerson/Maps/Egypt?listen"), TEXT("/Game/UI/Menu/Textures/EgyptMap"),TEXT("Egypt"), 3 });
}

void UUdemyMultiplayerGameInstance::InitializePlayerSpot()
{
    this->ConfigurationPlayerSpot.Add(1, FPlayerSpot{ 1, FVector(870,290,0), FRotator(0,90,0) });
    this->ConfigurationPlayerSpot.Add(2, FPlayerSpot{ 2, FVector(975,290,0), FRotator(0,90,0) });
    this->ConfigurationPlayerSpot.Add(3, FPlayerSpot{ 3, FVector(1080,290,0), FRotator(0,90,0) });
    this->ConfigurationPlayerSpot.Add(4, FPlayerSpot{ 4, FVector(1185,290,0), FRotator(0,90,0) });
}

void UUdemyMultiplayerGameInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UUdemyMultiplayerGameInstance, MaxPlayers);
	DOREPLIFETIME(UUdemyMultiplayerGameInstance, ServerName);
}
