// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "PlatformTrigger.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
#include "GameFramework/PlayerController.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer) {
	
	ConstructorHelpers::FClassFinder<UUserWidget> MenuClassBPClass(TEXT("/Game/ThirdPersonCPP/MenuSystem/WBP_MenuSystem"));
	if (!ensure(MenuClassBPClass.Class != nullptr)) { return; }
	MenuClass = MenuClassBPClass.Class;
	

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/ThirdPersonCPP/MenuSystem/InGameMenu"));
	if (!ensure(InGameMenuBPClass.Class != nullptr)) { return; }
	InGameMenuClass = InGameMenuBPClass.Class;
}

void UPuzzlePlatformsGameInstance::LoadMainMenu() {

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) { return; }
	PlayerController->ClientTravel("/Game/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
}


void UPuzzlePlatformsGameInstance::Init() {
	UE_LOG(LogTemp, Warning, TEXT("Found Class %s"), *MenuClass->GetName());
}


void UPuzzlePlatformsGameInstance::LoadMenu(){

	if (!ensure(MenuClass != nullptr)) { return; }
	Menu = CreateWidget<UMainMenu>(this, MenuClass);

	if (!ensure(Menu != nullptr)) { return; }
	Menu->Setup();
	Menu->SetMenuInterface(this);	

}

void UPuzzlePlatformsGameInstance::LoadInGameMenu() {

	if (!ensure(InGameMenuClass != nullptr)) { return; }
	UMenuWidget* Menu = CreateWidget<UMenuWidget>(this, InGameMenuClass);

	if (!ensure(Menu != nullptr)) { return; }
	Menu->Setup();
	Menu->SetMenuInterface(this);

}



void UPuzzlePlatformsGameInstance::Host() {	

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) { return; }

	Engine->AddOnScreenDebugMessage(-1, 4, FColor::Green, TEXT("Hosting"));
	UWorld* World = GetWorld();

	if (!ensure(World != nullptr)) { return; }
	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");

	Menu->Teardown();

}


void UPuzzlePlatformsGameInstance::Join(const FString& Address) {

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) { return; }

	Engine->AddOnScreenDebugMessage(-1, 3, FColor::Green, FString::Printf(TEXT("Joining %s"),*Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) { return; }
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);

	Menu->Teardown();
}

void UPuzzlePlatformsGameInstance::QuitGame() {

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) { return; }
	PlayerController->ConsoleCommand(TEXT("quit"), true);
}