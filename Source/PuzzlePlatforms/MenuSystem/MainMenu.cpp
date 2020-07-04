// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Button.h"
#include "WidgetSwitcher.h"
#include "EditableTextBox.h"
#include "PuzzlePlatformsGameInstance.h"

bool UMainMenu::Initialize() {

	bool Success = Super::Initialize();
	if (!Success) return false;
	
	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinMenu);

	if (!ensure(CancelButton != nullptr)) return false;
	CancelButton->OnClicked.AddDynamic(this, &UMainMenu::BackToMM);

	if (!ensure(JoinGameButton != nullptr)) return false;
	JoinGameButton->OnClicked.AddDynamic(this, &UMainMenu::JoinGame);

	return true;
}


void UMainMenu::JoinGame() {

	const FString& IpAddress = IPBox->Text.ToString();
	if (MenuInterface != nullptr) {
		MenuInterface->Join(IpAddress);
	}
}


void UMainMenu::HostServer()
{
	if (MenuInterface != nullptr) {
		MenuInterface->Host();	}

}


void UMainMenu::JoinMenu() {

	if (!ensure(MenuSwitcher != nullptr)) { return; }
	if (!ensure(Join_Menu != nullptr)) { return; }

	MenuSwitcher->SetActiveWidget(Join_Menu);
	
}





void UMainMenu::BackToMM() {

	if (!ensure(MenuSwitcher != nullptr)) { return; }
	if (!ensure(Main_Menu != nullptr)) { return; }

	MenuSwitcher->SetActiveWidget(Main_Menu);

}
