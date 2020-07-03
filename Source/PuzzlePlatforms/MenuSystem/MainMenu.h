// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;
	
public:

	void SetMenuInterface( IMenuInterface* MenuInterface);
	void Setup();
	void Teardown();

private:
	UFUNCTION()
	void HostServer();
	UPROPERTY(meta = (BindWidget))
	class UButton* Host;
	UPROPERTY(meta = (BindWidget))
	class UButton* Join;



	IMenuInterface* MenuInterface;
};
