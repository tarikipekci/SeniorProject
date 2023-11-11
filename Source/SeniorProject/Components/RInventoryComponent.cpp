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
	DOREPLIFETIME_CONDITION(URInventoryComponent, Items, COND_OwnerOnly);
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
		Location.X += FMath::RandRange(-50.0f, 100.0f);
		Location.Y += FMath::RandRange(-50.0f, 100.0f);
		FVector EndRay = Location;
		EndRay.Z -= 10000.0f;
		FHitResult HitResult;
		FCollisionObjectQueryParams ObjQuery;
		FCollisionQueryParams CollisionsParams;
		CollisionsParams.AddIgnoredActor(GetOwner());
		
		GetWorld()->LineTraceSingleByObjectType(OUT HitResult, Location, EndRay, ObjQuery, CollisionsParams);

		if(HitResult.ImpactPoint != FVector::ZeroVector)
		{
			Location = HitResult.ImpactPoint;
		}

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
		Items.Empty();
	}
}
