// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups.h"

#include "Components/StaticMeshComponent.h"
#include "SeniorProject/Characters/SeniorProjectCharacter.h"
#include "SeniorProject/Components/RPlayerStatComponent.h"

// Sets default values
APickups::APickups()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = MeshComp;
}

// Called when the game starts or when spawned
void APickups::BeginPlay()
{
	Super::BeginPlay();
	if(GetLocalRole() == ROLE_Authority)
	{
		SetReplicates(true);
	}
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
