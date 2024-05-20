// Copyright Epic Games, Inc. All Rights Reserved.

#include "SeniorProjectGameMode.h"
#include "UObject/ConstructorHelpers.h"

ASeniorProjectGameMode::ASeniorProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(
		TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if(PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ASeniorProjectGameMode::ReSpawn(AController* Controller)
{
	if(Controller)
	{
		if(HasAuthority())
		{
			FVector Location = FVector(3065.000000, -3811.000000, 360.000000);
			if(APawn* Pawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, Location, FRotator::ZeroRotator))
			{
				Controller->Possess(Pawn);
			}
		}
	}
}
