// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include "Button.h"


bool UInGameMenu::Initialize() {

	bool Success = Super::Initialize();

	if (!Success) { return false; }

	if (!ensure(B_Quit != nullptr)) { return false; }
	B_Quit->OnClicked.AddDynamic(this, &UInGameMenu::GoToMainMenu);

	if (!ensure(HostButton != nullptr)) { return false; }
	HostButton->OnClicked.AddDynamic(this, &UInGameMenu::CancelInGameMenu);

	return true;
}



void UInGameMenu::GoToMainMenu() {

	if (MenuInterface != nullptr)
	{
		Teardown();
		MenuInterface->LoadMainMenu();
	}
}

void UInGameMenu::CancelInGameMenu() {

	Teardown();

}

