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
	void Host();
	
	UFUNCTION(Exec)
	void Join( const FString& Adress);

	UFUNCTION(BlueprintCallable)
	void LoadMenu();
private:
	TSubclassOf<UUserWidget> MenuClass;
	class UMainMenu* Menu;
};
