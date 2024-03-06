// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryBuilding.h"

#include "Net/UnrealNetwork.h"
#include "SeniorProject/Components/RInventoryComponent.h"

class ARHUD;

void AInventoryBuilding::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicates to everyone
	DOREPLIFETIME_CONDITION(AInventoryBuilding, bOpened, COND_OwnerOnly);
}

AInventoryBuilding::AInventoryBuilding()
{
	InventoryComp = CreateDefaultSubobject<URInventoryComponent>("InventoryComponent");
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = MeshComp;
	bReplicates = true;
	SetReplicatingMovement(true);
	bOpened = false;
}

void AInventoryBuilding::Interact(ASeniorProjectCharacter* Player)
{
	if(Player->HasAuthority() && Player)
	{
		if(bOpened == false)
		{
			bOpened = true;
			Server_OpenInventory(Player);
		}
	}
}

void AInventoryBuilding::Server_OpenInventory_Implementation(ASeniorProjectCharacter* Player)
{
	Player->Client_OpenInventory();
}

void AInventoryBuilding::Server_CloseInventory_Implementation()
{
	bOpened = false;
	GEngine->AddOnScreenDebugMessage(-1,2,FColor::Red,"bopened set to false");
}
