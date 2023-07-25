// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DimensionInfo.generated.h"


/**
 * A class that contains information about a dimension
 */
USTRUCT(BlueprintType)
struct FDimensionInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector MapPosition;
};
