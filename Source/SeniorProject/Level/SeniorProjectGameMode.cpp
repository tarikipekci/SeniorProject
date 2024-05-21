// Copyright Epic Games, Inc. All Rights Reserved.

#include "SeniorProjectGameMode.h"

#include "EngineUtils.h"
#include "SpawnPoint.h"
#include "Algo/RandomShuffle.h"
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
	DefaultSpawnLocation = FVector((-1552.000000, -4395.000000, 360.000000));
	RespawnTimer = 6.0f;
}

void ASeniorProjectGameMode::BeginPlay()
{
	Super::BeginPlay();

	SpawnPoints.Empty();

	UClass* SpawnPointClass = ASpawnPoint::StaticClass();
	for(TActorIterator<AActor> Actor(GetWorld(), SpawnPointClass); Actor; ++Actor)
	{
		ASpawnPoint* SpawnPoint = Cast<ASpawnPoint>(*Actor);
		if(SpawnPoint)
		{
			SpawnPoints.Add(SpawnPoint);
		}
	}
}

void ASeniorProjectGameMode::Spawn(AController* Controller)
{
	if(!Controller || !DefaultPawnClass)
	{
		return;
	}

	ASpawnPoint* SpawnPoint = GetSpawnPoint();
	FVector Location = SpawnPoint ? SpawnPoint->GetActorLocation() : DefaultSpawnLocation;
	FRotator Rotation = SpawnPoint ? SpawnPoint->GetActorRotation() : FRotator::ZeroRotator;

	if(UWorld* World = GetWorld())
	{
		if(APawn* Pawn = World->SpawnActor<APawn>(DefaultPawnClass, Location, Rotation))
		{
			Controller->Possess(Pawn);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Pawn spawn failed."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GetWorld() returned nullptr."));
	}
}

void ASeniorProjectGameMode::ReSpawn(AController* Controller)
{
	if(Controller)
	{
		if(HasAuthority())
		{
			FTimerDelegate RespawnDelegate;
			RespawnDelegate.BindUFunction(this, FName("Spawn"), Controller);
			FTimerHandle RespawnHandle;
			GetWorld()->GetTimerManager().SetTimer(RespawnHandle, RespawnDelegate, RespawnTimer, false);
		}
	}
}

ASpawnPoint* ASeniorProjectGameMode::GetSpawnPoint()
{
	TArray<ASpawnPoint*> ShuffledSpawnPoints = SpawnPoints;
	Algo::RandomShuffle(ShuffledSpawnPoints);

	for(ASpawnPoint* SpawnPoint : ShuffledSpawnPoints)
	{
		if(SpawnPoint)
		{
			return SpawnPoint;
		}
	}
	return nullptr;
}
