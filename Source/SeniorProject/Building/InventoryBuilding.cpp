// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryBuilding.h"

#include "Net/UnrealNetwork.h"

void AInventoryBuilding::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicates to everyone
	DOREPLIFETIME_CONDITION(AInventoryBuilding, bOpened, COND_OwnerOnly);
}

AInventoryBuilding::AInventoryBuilding()
{
	bOpened = false;
}

void AInventoryBuilding::Interact(ASeniorProjectCharacter* Player)
{
	GEngine->AddOnScreenDebugMessage(-1,2,FColor::Red,"Interacted with chest");
	if(bOpened == false)
	{
		bOpened = true;
	}
}
