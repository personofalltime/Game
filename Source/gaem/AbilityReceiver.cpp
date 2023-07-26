// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitiesSubsystem.h"
#include "AbilityReceiver.h"

// Sets default values for this component's properties
UAbilityReceiver::UAbilityReceiver()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UAbilityReceiver::BeginPlay()
{
	Super::BeginPlay();

	DimensionNumber = 0;
}

void UAbilityReceiver::MoveToDimension(int32 NewDimensionNumber) {
	if (DimensionNumber == NewDimensionNumber) return;
	AActor* owner = GetOwner();
    UAbilitiesSubsystem* abilitiesSubsystem = owner->GetGameInstance()->GetSubsystem<UAbilitiesSubsystem>();
	FVector currentDimensionPosition = abilitiesSubsystem->GameSettings->DimensionInfos[DimensionNumber].DimensionTrigger->GetActorLocation();
	FVector newDimensionPosition = abilitiesSubsystem->GameSettings->DimensionInfos[NewDimensionNumber].DimensionTrigger->GetActorLocation();
	// Find the vector from the current dimension to the new dimension
	FVector displacementVector = newDimensionPosition - currentDimensionPosition;

	// Change the position
	owner->SetActorLocation(owner->GetActorLocation() + displacementVector);

	// Change the dimension
	DimensionNumber = NewDimensionNumber;
}