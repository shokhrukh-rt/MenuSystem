// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "OnlineSessionSettings.h"
#include "OnlineSessionInterface.h"


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
		}
	
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Subsystem is not found"));
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




// Join
void UPuzzlePlatformsGameInstance::Join(const FString& Address) {

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) { return; }

	Engine->AddOnScreenDebugMessage(-1, 3, FColor::Green, FString::Printf(TEXT("Joining %s"),*Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) { return; }
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);

	Menu->Teardown();
}


// QuitGame
void UPuzzlePlatformsGameInstance::QuitGame() {

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) { return; }
	PlayerController->ConsoleCommand(TEXT("quit"), true);
}