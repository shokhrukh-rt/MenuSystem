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

}

