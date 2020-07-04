// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "PuzzlePlatformsGameInstance.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
public:	

	UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer);
	void Init();
	
	UFUNCTION(Exec)
	void Host() override;
	
	UFUNCTION(Exec)
	void Join( const FString& Address) override;

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();

	virtual void LoadMainMenu() override;
	virtual void QuitGame() override;


private:
	TSubclassOf<UUserWidget> MenuClass;
	class UMainMenu* Menu;

	TSubclassOf<UUserWidget> InGameMenuClass;
	
};
