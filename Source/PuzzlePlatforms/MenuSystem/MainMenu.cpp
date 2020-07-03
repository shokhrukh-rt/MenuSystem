// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Button.h"
#include "PuzzlePlatformsGameInstance.h"

bool UMainMenu::Initialize() {

	bool Success = Super::Initialize();
	if (!Success) return false;
	
	if (!ensure(Host != nullptr)) return false;

	Host->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	return true;
}

void UMainMenu::HostServer()
{
	UE_LOG(LogTemp, Warning, TEXT("HOST button is clicked!"));

	if (MenuInterface != nullptr) {
		MenuInterface->Host();
	}


}

void UMainMenu::SetMenuInterface(IMenuInterface* MenuInterface) {
	this->MenuInterface = MenuInterface;
}

void UMainMenu::Setup() {

	this->AddToViewport();
	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) { return; }

	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetWidgetToFocus(this->TakeWidget());


	PlayerController->bShowMouseCursor = true;
	PlayerController->SetInputMode(InputMode);
}


void UMainMenu::Teardown() {

	this->RemoveFromViewport();

	FInputModeGameOnly InputMode;
	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) { return; }
	PlayerController->SetInputMode(InputMode);

	PlayerController->bShowMouseCursor = false;
	
}