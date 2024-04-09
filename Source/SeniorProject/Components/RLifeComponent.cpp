// Fill out your copyright notice in the Description page of Project Settings.


#include "RLifeComponent.h"

// Sets default values for this component's properties
URLifeComponent::URLifeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void URLifeComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHitPoints = MaxHitPoints;
}


// Called every frame
void URLifeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URLifeComponent::DecreaseHitPoints(int DecreasedAmount)
{
	if(CurrentHitPoints - DecreasedAmount <= 0)
	{
		GetOwner()->Destroy();
	}
	else
	{
		CurrentHitPoints -= DecreasedAmount;
	}
}

