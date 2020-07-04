// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UInGameMenu : public UMenuWidget
{
	GENERATED_BODY()/*

public:
	void LoadInGameMenu();

protected:
	virtual bool Initialize() override;

private:
	
	//Buttons and Widgets
	UPROPERTY(meta = (BindWidget))
	class UButton* B_GoToMainMenu;

	UPROPERTY(meta = (BindWidget))
	UButton* B_CancelInGameMenu;

	//Callback Functions

	UFUNCTION()
	void GoToMainMenu();

	UFUNCTION()
	void CancelInGameMenu();


	*/
};
