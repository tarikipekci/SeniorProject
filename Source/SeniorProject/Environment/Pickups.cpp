// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups.h"

#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "SeniorProject/Characters/SeniorProjectCharacter.h"
#include "SeniorProject/Components/RPlayerStatComponent.h"

// Sets default values
APickups::APickups()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = MeshComp;
	bReplicates = true;
	SetReplicatingMovement(true);
	bObjectPickedUp = false;
}

void APickups::OnRep_PickedUp()
{
	this->MeshComp->SetHiddenInGame(bObjectPickedUp);
	this->SetActorEnableCollision(!bObjectPickedUp);
}

// Called when the game starts or when spawned
void APickups::BeginPlay()
{
	Super::BeginPlay();
}

void APickups::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicates to everyone
	DOREPLIFETIME(APickups, bObjectPickedUp);
}

void APickups::UseItem(ASeniorProjectCharacter* Player)
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

void APickups::InInventory(bool In)
{
	if(GetLocalRole() == ROLE_Authority)
	{
		bObjectPickedUp = In;
		OnRep_PickedUp();
	}
}
