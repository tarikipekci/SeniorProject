// Fill out your copyright notice in the Description page of Project Settings.


#include "DayNightCycleManager.h"

#include "Engine/DirectionalLight.h"

// Sets default values
ADayNightCycleManager::ADayNightCycleManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ADayNightCycleManager::PerformDayNightCycle(float DeltaTime)
{
	if(LightSource)
	{
		LightSource->AddActorLocalRotation(FRotator(TurnRate * DeltaTime, 0, 0));
	}
}

// Called when the game starts or when spawned
void ADayNightCycleManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADayNightCycleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PerformDayNightCycle(DeltaTime);
}
