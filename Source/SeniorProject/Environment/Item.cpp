// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "SeniorProject/Characters/SeniorProjectCharacter.h"
#include "SeniorProject/Components/RInventoryComponent.h"
#include "SeniorProject/Components/RPlayerStatComponent.h"

// Sets default values
AItem::AItem()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = MeshComp;
	InteractCollision = CreateDefaultSubobject<UCapsuleComponent>("InteractCollision");
	InteractCollision->SetupAttachment(MeshComp);
    InteractCollision->SetRelativeLocation(FVector::ZeroVector);
	bReplicates = true;
	SetReplicatingMovement(true);
	bIsInteractable = true;
	ItemData.ItemActor = this;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	InteractCollision->SetCollisionProfileName("InteractCollision");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bIsEquipped = false;
}

void AItem::Interact(ASeniorProjectCharacter* Player)
{
	if(HasAuthority() && Player)
	{
		if(Player->InventoryComp->AddItem(ItemData))
		{
			HideItem();
		}
	}
}

void AItem::Use(ASeniorProjectCharacter* Player)
{
	IInteractableInterface::Use(Player);

	if(ItemType == EItemType::Edible)
	{
		Player->PlayerStatComp->DecreaseHunger(ChangeAmount);
	}
	else if(ItemType == EItemType::Drinkable)
	{
		Player->PlayerStatComp->DecreaseThirst(ChangeAmount);
	}
	else if(ItemType == EItemType::Health)
	{
		Player->PlayerStatComp->IncreaseHealth(ChangeAmount);
	}
	else if(ItemType == EItemType::Tool)
	{
		if(Player->InventoryComp->GetEquippedItem())
		{
			if(Player->InventoryComp->GetEquippedItem() == this)
			{
				Player->UnEquipItem(this);
			}
		}
		else
		{
			Player->EquipItem(this);
		}
	}
}

void AItem::HideItem_Implementation()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	MeshComp->SetSimulatePhysics(false);
}

void AItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicates to everyone
	DOREPLIFETIME(AItem,bIsInteractable);
	DOREPLIFETIME(AItem,bIsEquipped);
}
