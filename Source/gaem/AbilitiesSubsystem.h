// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameSettings.h"
#include "AbilitiesSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class GAEM_API UAbilitiesSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	public:

    
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	AGameSettings* GameSettings;
	
    // Begin USubsystem
    // virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    // virtual void Deinitialize() override;
    // End USubsystem

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = State, meta = (AllowPrivateAccess = "true"))
	int32 PlayerDimensionNumber;
};
