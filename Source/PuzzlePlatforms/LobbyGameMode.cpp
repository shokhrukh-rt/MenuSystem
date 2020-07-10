// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "PuzzlePlatformsGameInstance.h"
#include "TimerManager.h"



// PostLogin
void ALobbyGameMode::PostLogin(APlayerController* NewPlayer) {

	Super::PostLogin(NewPlayer);

	++NumOfPlayers;
	if (NumOfPlayers >= 2) {
		UE_LOG(LogTemp, Warning, TEXT("Reached 3 players"));
		
		GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &ALobbyGameMode::StartGame, 10.0f);		
	}
}


// Logout
void ALobbyGameMode::Logout(AController* Exiting) {

	Super::Logout(Exiting);
	--NumOfPlayers;
}


// StartGame
void ALobbyGameMode::StartGame() {

	auto GameInstance = Cast<UPuzzlePlatformsGameInstance>(GetGameInstance());

	if (GameInstance == nullptr) { return; }
	GameInstance->StartSession();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) { return; }

	bUseSeamlessTravel = true;
	World->ServerTravel("/Game/ThirdPersonCPP/Maps/GameMap?listen");

}