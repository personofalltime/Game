// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DimensionTriggerBox.h"
#include "DimensionInfo.generated.h"


/**
 * A class that contains information about a dimension
 */
USTRUCT(BlueprintType)
struct FDimensionInfo
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	class ADimensionTriggerBox* DimensionTrigger;
};
