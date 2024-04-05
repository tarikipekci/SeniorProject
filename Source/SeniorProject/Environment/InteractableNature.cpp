// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableNature.h"

#include "Item.h"
#include "SeniorProject/Components/RInventoryComponent.h"

// Sets default values
AInteractableNature::AInteractableNature()
{
}

// Called when the game starts or when spawned
void AInteractableNature::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AInteractableNature::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractableNature::Interact(ASeniorProjectCharacter* Player)
{
	if(Player->GetCanFillWater())
	{
		AItem* RewardItem = GetWorld()->SpawnActor<AItem>(RewardItemClass, FVector::ZeroVector, FRotator(0, 0, 0));
		Player->InventoryComp->ChangeItemFromInventory(Player->InventoryComp->GetEquippedItem()->ItemData,
		                                               RewardItem->ItemData);
		Player->UnEquipItem(Player->InventoryComp->GetEquippedItem());
		RewardItem->SetActorHiddenInGame(true);
		bIsInteractable = false;
	}
}

void AInteractableNature::Use(ASeniorProjectCharacter* Player)
{
	IInteractableInterface::Use(Player);
}
