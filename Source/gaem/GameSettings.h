// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DimensionInfo.h"
#include "GameFramework/Actor.h"
#include "GameSettings.generated.h"

/**
 * An actor that is instantiated in a level and stores settings for that level
*/

UCLASS()
class GAEM_API AGameSettings : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dimensions, meta = (AllowPrivateAccess = "true"))
	TArray<FDimensionInfo> DimensionInfos;

	// Sets default values for this actor's properties
	AGameSettings();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
