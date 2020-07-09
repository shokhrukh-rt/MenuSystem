// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "OnlineSessionSettings.h"

#include "PlatformTrigger.h"
#include "MenuSystem/MainMenu.h"
#include "GameFramework/PlayerController.h"


// Current Session Name
const static FName SESSION_NAME = TEXT("Current Session");


// Constructor
UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer) {
	
	ConstructorHelpers::FClassFinder<UUserWidget> MenuClassBPClass(TEXT("/Game/ThirdPersonCPP/MenuSystem/WBP_MenuSystem"));
	if (!ensure(MenuClassBPClass.Class != nullptr)) { return; }
	MenuClass = MenuClassBPClass.Class;
	

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/ThirdPersonCPP/MenuSystem/InGameMenu"));
	if (!ensure(InGameMenuBPClass.Class != nullptr)) { return; }
	InGameMenuClass = InGameMenuBPClass.Class;
}



// LoadMainMenu
void UPuzzlePlatformsGameInstance::LoadMainMenu() {

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) { return; }
	PlayerController->ClientTravel("/Game/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
}


// Init
void UPuzzlePlatformsGameInstance::Init() {

	UE_LOG(LogTemp, Warning, TEXT("Found Class %s"), *MenuClass->GetName());

	IOnlineSubsystem* Subsystem  = IOnlineSubsystem::Get();
	if ( Subsystem != nullptr) { 
		UE_LOG(LogTemp, Warning, TEXT("Subsystem %s is found"), *Subsystem->GetSubsystemName().ToString());

		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid()) {
			
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionsComplete);	
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnJoinSessionComplete);
		}
	
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Subsystem is not found"));
	}	
}


// RequestRefresh
void UPuzzlePlatformsGameInstance::RequestRefresh() {
	FindSessions();
}


// FindSessions
void UPuzzlePlatformsGameInstance::FindSessions() {

	SearchSettings = MakeShareable(new FOnlineSessionSearch());
	if (SearchSettings.IsValid()) {

		SearchSettings->bIsLanQuery = true;
		SearchSettings->MaxSearchResults = 100;
		SearchSettings->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

		UE_LOG(LogTemp, Warning, TEXT("Starting Find Session"));
		SessionInterface->FindSessions(0, SearchSettings.ToSharedRef());
	}
}


// LoadMenuWidget
void UPuzzlePlatformsGameInstance::LoadMenuWidget(){

	if (!ensure(MenuClass != nullptr)) { return; }
	Menu = CreateWidget<UMainMenu>(this, MenuClass);

	if (!ensure(Menu != nullptr)) { return; }
	Menu->Setup();
	Menu->SetMenuInterface(this);	

}


// LoadInGameMenu
void UPuzzlePlatformsGameInstance::LoadInGameMenu() {

	if (!ensure(InGameMenuClass != nullptr)) { return; }
	UMenuWidget* Menu = CreateWidget<UMenuWidget>(this, InGameMenuClass);

	if (!ensure(Menu != nullptr)) { return; }
	Menu->Setup();
	Menu->SetMenuInterface(this);

}


// Host
void UPuzzlePlatformsGameInstance::Host() {

	if (SessionInterface.IsValid()) {
		
		auto CurrentSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (CurrentSession != nullptr)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else {

			CreateSession();
		}
		
	}
	
}


// CreateSession
void UPuzzlePlatformsGameInstance::CreateSession() {

	if (SessionInterface.IsValid()) {

		FOnlineSessionSettings SessionSettings;

		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL") {

			SessionSettings.bIsLANMatch = true;
		}
		else {

			SessionSettings.bIsLANMatch = false;
		}
		
		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
	
}


// OnDestroySessionComplete
void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool Success) {

	if (Success)
	{
		CreateSession();
	}
	
}


// OnCreateSessionComplete
void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool Success){

	if (!Success) {

		UE_LOG(LogTemp, Warning, TEXT("Could not create Session"));
		return;
	}

		UEngine* Engine = GetEngine();
		if (!ensure(Engine != nullptr)) { return; }

		Engine->AddOnScreenDebugMessage(-1, 4, FColor::Green, TEXT("Hosting"));
		UWorld* World = GetWorld();

		if (!ensure(World != nullptr)) { return; }
		World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");

		Menu->Teardown();
}


// OnFindSessionsComplete
void UPuzzlePlatformsGameInstance::OnFindSessionsComplete(bool Success) {

	if (Success && SearchSettings.IsValid() && Menu != nullptr) {
		auto SearchResults = SearchSettings->SearchResults;

		TArray<FServerData> ServerNames;

			for (const auto& Element : SearchResults) {

				UE_LOG(LogTemp, Warning, TEXT("Find Session Finished"));
				UE_LOG(LogTemp, Warning, TEXT("Found Session Name: %s"), *Element.GetSessionIdStr());
				FServerData Data;
				
				Data.Name = Element.GetSessionIdStr();
				Data.CurrentPlayers = Element.Session.NumOpenPublicConnections;
				Data.MaxPlayers = Element.Session.SessionSettings.NumPublicConnections;
				Data.HostUsername = Element.Session.OwningUserName;
				ServerNames.Add(Data);
				
			}
			Menu->SetServerList(ServerNames);
			
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Online Sessions are NOT found"));
	}

}


// Join
void UPuzzlePlatformsGameInstance::Join(uint32 Index) {

	if (!SessionInterface.IsValid()) { return; }

	SessionInterface->JoinSession(0, SESSION_NAME, SearchSettings->SearchResults[Index]);

	Menu->Teardown();
}



// OnJoinSessionComplete
void UPuzzlePlatformsGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) {

	if (!SessionInterface.IsValid()) { return; }

	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address)) {
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect STRING"));
		return;
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) { return; }

	Engine->AddOnScreenDebugMessage(-1, 3, FColor::Green, FString::Printf(TEXT("Joining %s"),*Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) { return; }
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

// QuitGame
void UPuzzlePlatformsGameInstance::QuitGame() {

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) { return; }
	PlayerController->ConsoleCommand(TEXT("quit"), true);
}