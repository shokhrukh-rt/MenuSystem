// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"


USTRUCT()
struct FServerData 
{
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUsername;
};

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);
	void SetServerList(TArray<FServerData> ServerName);
	void SetIndex(uint32);
protected:
	virtual bool Initialize() override;
	
private:	

	// CallBack Functions
	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinMenu();

	UFUNCTION()
	void BackToMM();

	UFUNCTION()
	void JoinGame();

	UFUNCTION()
	void QuitGame();

	void UpdateChildren();


	//Bind Widgets and Buttons
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinGameButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	UWidget* Join_Menu;

	UPROPERTY(meta = (BindWidget))
	UWidget* Main_Menu;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ServerList;

	TOptional<uint32> SelectedIndex;

	TSubclassOf<UUserWidget> ServerRowClass;
	
};
