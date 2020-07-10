// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

// PostLogin
void ALobbyGameMode::PostLogin(APlayerController* NewPlayer) {

	Super::PostLogin(NewPlayer);

	++NumOfPlayers;
	if (NumOfPlayers >= 3) {
		UE_LOG(LogTemp, Warning, TEXT("Reached 3 players"));

		UWorld* World = GetWorld();
		if (!ensure(World != nullptr)) { return; }

		bUseSeamlessTravel = true;
		World->ServerTravel("/Game/ThirdPersonCPP/Maps/GameMap?listen");
	}
}

void ALobbyGameMode::Logout(AController* Exiting) {

	Super::Logout(Exiting);
	--NumOfPlayers;
}