// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform() {
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);

}

void AMovingPlatform::BeginPlay() {
	Super::BeginPlay();
	if (HasAuthority()) {

		SetReplicates(true);
		SetReplicateMovement(true);

		GlobalStartLocation = GetActorLocation();
		GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
	}
	
}

void AMovingPlatform::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);
	if (ActiveTrigger > 0)
	{
		if (HasAuthority()) {

			FVector Location = GetActorLocation();
			float JourneyTravelled = (Location - GlobalStartLocation).Size();
			float JourneyLength = (GlobalTargetLocation - GlobalStartLocation).Size();

			if (JourneyTravelled >= JourneyLength) {
				FVector Swap = GlobalStartLocation;
				GlobalStartLocation = GlobalTargetLocation;
				GlobalTargetLocation = Swap;
			}


			FVector Direction = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
			Location += Speed * DeltaTime * Direction;
			SetActorLocation(Location);



		}


	}
	
	
}

void AMovingPlatform::AddActiveTrigger() {

	ActiveTrigger++;

}


void AMovingPlatform::RemoveActiveTrigger() {

	if (ActiveTrigger > 0)
		ActiveTrigger--;
}
