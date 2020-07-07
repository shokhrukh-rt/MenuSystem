// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"

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
	void LoadMenuWidget();

	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();

	virtual void LoadMainMenu() override;
	virtual void QuitGame() override;
	virtual void RequestRefresh() override;
	void FindSessions();


private:
	TSubclassOf<UUserWidget> MenuClass;
	class UMainMenu* Menu;

	TSubclassOf<UUserWidget> InGameMenuClass;
	IOnlineSessionPtr SessionInterface;

	TSharedPtr<class FOnlineSessionSearch> SearchSettings;

	void OnCreateSessionComplete(FName, bool);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void CreateSession();
	
};
