// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"

void UMenuWidget::SetMenuInterface(IMenuInterface* MenuInterface) {
	this->MenuInterface = MenuInterface;
}


void UMenuWidget::Setup() {

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


void UMenuWidget::Teardown() {

	this->RemoveFromViewport();

	FInputModeGameOnly InputMode;
	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) { return; }
	PlayerController->SetInputMode(InputMode);

	PlayerController->bShowMouseCursor = false;

}