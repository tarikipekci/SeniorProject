// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SeniorProjectGameMode.generated.h"

class AInventoryBuilding;
class ASpawnPoint;

UCLASS(minimalapi)
class ASeniorProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASeniorProjectGameMode();
	virtual void BeginPlay() override;

	UFUNCTION()
	void Spawn(AController* Controller);

	UFUNCTION()
	void ReSpawn(AController* Controller);

protected:
	TArray<ASpawnPoint*> SpawnPoints;
	FVector DefaultSpawnLocation;

	UPROPERTY(EditAnywhere, Category="Respawn")
	float RespawnTimer;

protected:
	ASpawnPoint* GetSpawnPoint();
};



