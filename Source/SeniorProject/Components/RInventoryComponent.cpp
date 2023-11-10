// Fill out your copyright notice in the Description page of Project Settings.


#include "RInventoryComponent.h"

#include "Net/UnrealNetwork.h"
#include "SeniorProject/Environment/Pickups.h"
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


bool URInventoryComponent::AddItem(APickups* Item)
{
	Items.Add(Item);
	Item->InInventory(true);

	for(APickups* Pickup : Items)
	{
		FString Tempstr = "";
		FString str = Tempstr.Append(Pickup->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, str);
	}
	return false;
}

void URInventoryComponent::DropItem(APickups* Item)
{
	if(GetOwnerRole() == ROLE_Authority)
	{
		FVector Location = GetOwner()->GetActorLocation();
		Item->SetActorLocation(Location);
		Item->InInventory(false);
	}
}

void URInventoryComponent::DropAllInventoryItems()
{
	if(GetOwnerRole() == ROLE_Authority)
	{
		for(APickups* Pickup : Items)
		{
			DropItem(Pickup);
		}
	}
}
