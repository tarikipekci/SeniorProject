// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "SeniorProject/Characters/SeniorProjectCharacter.h"
#include "SeniorProject/Components/RPlayerStatComponent.h"

// Sets default values
AItem::AItem()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = MeshComp;
	bReplicates = true;
	SetReplicatingMovement(true);
	bObjectPickedUp = false;
}

void AItem::OnRep_PickedUp()
{
	this->MeshComp->SetHiddenInGame(bObjectPickedUp);
	this->SetActorEnableCollision(!bObjectPickedUp);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
}

void AItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicates to everyone
	DOREPLIFETIME(AItem, bObjectPickedUp);
}

void AItem::UseItem(ASeniorProjectCharacter* Player)
{
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
		Destroy();
	}
}

void AItem::InInventory(bool In)
{
	if(GetLocalRole() == ROLE_Authority)
	{
		bObjectPickedUp = In;
		OnRep_PickedUp();
	}
}

void AItem::Interact(ASeniorProjectCharacter* Player)
{
	if(Player)
	{
		Player->ItemPickedUp.Broadcast(ItemData);
	}
	Destroy();
}
