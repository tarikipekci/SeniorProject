// Copyright Epic Games, Inc. All Rights Reserved.

#include "SeniorProjectGameMode.h"
#include "UObject/ConstructorHelpers.h"

ASeniorProjectGameMode::ASeniorProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
