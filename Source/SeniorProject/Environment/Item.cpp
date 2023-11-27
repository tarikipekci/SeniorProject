// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "Components/StaticMeshComponent.h"
#include "SeniorProject/Characters/SeniorProjectCharacter.h"
#include "SeniorProject/Components/RInventoryComponent.h"
#include "SeniorProject/Components/RPlayerStatComponent.h"

// Sets default values
AItem::AItem()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = MeshComp;
	bReplicates = true;
	SetReplicatingMovement(true);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
}

void AItem::Interact(ASeniorProjectCharacter* Player)
{
	if(HasAuthority() && Player)
	{
		Player->InventoryComp->AddItem(ItemData);
		Destroy();
	}
}

void AItem::Use(ASeniorProjectCharacter* Player)
{
	IInteractableInterface::Use(Player);

	if(GetLocalRole() == ROLE_Authority)
	{
		if(PickupType == EPickupItemType::EEdible)
		{
			Player->PlayerStatComp->DecreaseHunger(ChangeAmount);
		}
		else if(PickupType == EPickupItemType::EDrinkable)
		{
			Player->PlayerStatComp->DecreaseThirst(ChangeAmount);
		}
		else if(PickupType == EPickupItemType::EHealth)
		{
			Player->PlayerStatComp->IncreaseHealth(ChangeAmount);
		}
	}
}
