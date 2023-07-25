// Copyright Epic Games, Inc. All Rights Reserved.

#include "gaemGameMode.h"
#include "gaemCharacter.h"
#include "UObject/ConstructorHelpers.h"

AgaemGameMode::AgaemGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}