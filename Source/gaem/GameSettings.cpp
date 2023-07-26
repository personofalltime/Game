// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitiesSubsystem.h"
#include "GameSettings.h"

// Sets default values
AGameSettings::AGameSettings()
{
 	// Do not tick to not introduce lag
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AGameSettings::BeginPlay()
{
	Super::BeginPlay();

	// Set itself as the game settings for the gamemode
	GetGameInstance()->GetSubsystem<UAbilitiesSubsystem>()->GameSettings = this;

	// For each trigger, set its dimension number
	for (int i = 0; i < DimensionInfos.Num(); i++) {
		DimensionInfos[i].DimensionTrigger->DimensionNumber = i;
	}
}
