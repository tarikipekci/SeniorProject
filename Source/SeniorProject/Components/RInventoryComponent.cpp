// Fill out your copyright notice in the Description page of Project Settings.


#include "RInventoryComponent.h"

#include "Net/UnrealNetwork.h"
// Sets default values for this component's properties
URInventoryComponent::URInventoryComponent()
{
}


// Called when the game starts
void URInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void URInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicates to everyone
	DOREPLIFETIME(URInventoryComponent, Items);
}
