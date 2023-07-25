// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DimensionInfo.h"
#include "gaemGameMode.generated.h"

UCLASS(minimalapi)
class AgaemGameMode : public AGameModeBase
{
	GENERATED_BODY()	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dimensions, meta = (AllowPrivateAccess = "true"))
	struct FDimensionInfo Dimension1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dimensions, meta = (AllowPrivateAccess = "true"))
	struct FDimensionInfo Dimension2;

	AgaemGameMode();	

};
