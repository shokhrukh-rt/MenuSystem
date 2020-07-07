// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRow.h"
#include "TextBlock.h"
#include "Button.h"
#include "MainMenu.h"


// ButtonCallback
void UServerRow::ButtonCallback() {
	Parent->SetIndex(Index);
}

// Setup
void UServerRow::Setup( UMainMenu* InParent, uint32 InIndex){

	Parent = InParent;
	Index = InIndex;

	Button_0->OnClicked.AddDynamic(this, &UServerRow::ButtonCallback);
}

