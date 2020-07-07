// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "UObject/ConstructorHelpers.h"
#include "Button.h"
#include "WidgetSwitcher.h"
#include "EditableTextBox.h"
#include "TextBlock.h"
#include "ServerRow.h"


#include "PuzzlePlatformsGameInstance.h"

// Contructor
UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer) {

	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/ThirdPersonCPP/MenuSystem/WBP_ServerRow"));
	if (!ensure(ServerRowBPClass.Class != nullptr)) { return; }
	ServerRowClass = ServerRowBPClass.Class;
}

// Initialize
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

	if (!ensure(QuitButton != nullptr)) return false;
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);


	return true;
}

void UMainMenu::QuitGame() {

	MenuInterface->QuitGame();

}


// JoinGame
void UMainMenu::JoinGame() {

	
	MenuInterface->Join("");

}


// void SetServerList(TArray<FString> ServerName);
void UMainMenu::SetServerList(TArray<FString> ServerNames) {

	ServerList->ClearChildren();

	for (const FString& ServerName : ServerNames) {

		if (!ensure(ServerRowClass != nullptr)) { return; }
		UServerRow* Row = CreateWidget<UServerRow>(this, ServerRowClass);

		Row->ServerName->SetText(FText::FromString(ServerName));
		ServerList->AddChild(Row);

	}

	
}


void UMainMenu::HostServer()
{
	if (MenuInterface != nullptr) {
		MenuInterface->Host();	}

}


// JoinMenu
void UMainMenu::JoinMenu() {

	if (!ensure(MenuSwitcher != nullptr)) { return; }
	if (!ensure(Join_Menu != nullptr)) { return; }

	MenuSwitcher->SetActiveWidget(Join_Menu);

	if (MenuInterface != nullptr)
	{
		MenuInterface->RequestRefresh();
	}
	
}





void UMainMenu::BackToMM() {

	if (!ensure(MenuSwitcher != nullptr)) { return; }
	if (!ensure(Main_Menu != nullptr)) { return; }

	MenuSwitcher->SetActiveWidget(Main_Menu);

}
