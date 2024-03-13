// Fill out your copyright notice in the Description page of Project Settings.


#include "RInventoryComponent.h"

#include "RInteractComponent.h"
#include "Net/UnrealNetwork.h"
#include "../Environment/Item.h"
#include "SeniorProject/Building/InventoryBuilding.h"

class ARHUD;
// Sets default values for this component's properties
URInventoryComponent::URInventoryComponent()
{
	InventoryMaxSlotSize = 12;
	DropRange = 100;
	DropDepth = -200;
}

// Called when the game starts
void URInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<ASeniorProjectCharacter>(GetOwner());
	SetIsReplicated(true);
	InitializeInventory();
}

bool URInventoryComponent::AddItem(FItemData ItemData)
{
	bool bAdded = false;
	int EmptySlotIndex = -1;
	if(GetOwner()->HasAuthority())
	{
		if(!ItemData.ItemClass)
		{
			return false;
		}

		for(int i = 0; i < InventoryItems.Num(); i++)
		{
			FItemData& Item = InventoryItems[i];

			if(!Item.ItemClass && EmptySlotIndex == -1)
			{
				EmptySlotIndex = i;
			}

			if(Item.ItemClass == ItemData.ItemClass)
			{
				int AvailableSize = Item.MaxStackSize - Item.CurrentStackCount;
				int NeededSize = ItemData.CurrentStackCount;

				if(AvailableSize <= 0)
				{
					continue;
				}

				if(AvailableSize >= NeededSize)
				{
					Item.CurrentStackCount += ItemData.CurrentStackCount;
					bAdded = true;
					break;
				}

				int RemainingCount = NeededSize - AvailableSize;
				Item.CurrentStackCount = Item.MaxStackSize;
				ItemData.CurrentStackCount = RemainingCount;
			}
		}

		if(!bAdded && EmptySlotIndex >= 0)
		{
			InventoryItems[EmptySlotIndex] = ItemData;
			bAdded = true;
		}
	}
	else
	{
		Server_AddItem(ItemData);
	}
	if(bAdded)
	{
		OnRep_InventoryUpdated();
	}
	return bAdded;
}

void URInventoryComponent::Server_AddItem_Implementation(FItemData ItemData)
{
	AddItem(ItemData);
}

void URInventoryComponent::SwapItems(URInventoryComponent* TargetInventory, int Index1, int Index2)
{
	if(GetOwner()->HasAuthority())
	{
		if(Index1 >= 0 && Index1 < InventoryMaxSlotSize && Index2 >= 0 && Index2 < InventoryMaxSlotSize)
		{
			FItemData* Item1 = &InventoryItems[Index1];
			FItemData* Item2 = &TargetInventory->InventoryItems[Index2];

			if(Item1->ItemClass == Item2->ItemClass)
			{
				int AvailableSize = Item2->MaxStackSize - Item2->CurrentStackCount;

				if(Item1->CurrentStackCount <= AvailableSize)
				{
					Item2->CurrentStackCount += Item1->CurrentStackCount;
					*Item1 = FItemData();
				}
				else
				{
					int DraggedItemRemaining = Item1->CurrentStackCount - AvailableSize;
					Item1->CurrentStackCount = DraggedItemRemaining;
					Item2->CurrentStackCount += AvailableSize;
				}
			}
			else
			{
				FItemData TempItem = *Item1;
				InventoryItems[Index1] = *Item2;
				TargetInventory->InventoryItems[Index2] = TempItem;
			}
		}
		OnRep_InventoryUpdated();
	}
	else
	{
		Server_SwapItems(TargetInventory, Index1, Index2);
	}
}

void URInventoryComponent::Server_SwapItems_Implementation(URInventoryComponent* TargetInventory, int Index1,
                                                           int Index2)
{
	SwapItems(TargetInventory, Index1, Index2);
}

void URInventoryComponent::OnRep_InventoryUpdated()
{
	InventoryUpdated.Broadcast(InventoryItems);
}

void URInventoryComponent::DecreaseItemAmount(int SlotIndex, int Amount)
{
	if(Player->HasAuthority())
	{
		if(InventoryItems[SlotIndex].ItemClass)
		{
			InventoryItems[SlotIndex].CurrentStackCount -= Amount;
			if(InventoryItems[SlotIndex].CurrentStackCount <= 0)
			{
				FItemData& CurrentItemData = InventoryItems[SlotIndex];
				CurrentItemData = FItemData();
			}
		}
	}
	else
	{
		Server_DecreaseItemAmount(SlotIndex, Amount);
	}

	OnRep_InventoryUpdated();
}

void URInventoryComponent::Server_DecreaseItemAmount_Implementation(int SlotIndex, int Amount)
{
	DecreaseItemAmount(SlotIndex, Amount);
}

void URInventoryComponent::InitializeInventory()
{
	InventoryItems.Reserve(InventoryMaxSlotSize);

	for(int i = 0; i < InventoryMaxSlotSize; i++)
	{
		InventoryItems.Emplace();
	}
}

void URInventoryComponent::UseInventoryItem(int SlotIndex)
{
	if(Player)
	{
		Player->UsePickup(InventoryItems[SlotIndex].ItemActor);
		DecreaseItemAmount(SlotIndex, 1);
	}
}

void URInventoryComponent::DropInventoryItem(int SlotIndex)
{
	if(GetOwner()->HasAuthority())
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

		if(PlayerController)
		{
			FVector Start = Player->InteractComp->GetComponentLocation() + Player->GetActorForwardVector() * DropRange;
			FVector End = Start + FVector(0, 0, DropDepth);
			FHitResult HitResult;
			FCollisionQueryParams CollisionQueryParams;

			if(GetWorld()->LineTraceSingleByChannel(OUT HitResult, Start, End, ECC_GameTraceChannel1,
			                                        CollisionQueryParams))
			{
				AActor* Actor = HitResult.GetActor();
				AActor* Item = InventoryItems[SlotIndex].ItemActor;
				if(Item)
				{
					NetMulticast_DropItem(SlotIndex, Item, HitResult.ImpactPoint);
					DecreaseItemAmount(SlotIndex, InventoryItems[SlotIndex].CurrentStackCount);
				}
				else
				{
					NetMulticast_SpawnItem(SlotIndex, Item, HitResult.ImpactPoint);
				}
			}
		}
	}
	else
	{
		Server_DropInventoryItem(SlotIndex);
	}
}

void URInventoryComponent::NetMulticast_SpawnItem_Implementation(int SlotIndex, AActor* Item, FVector Location)
{
	AItem* DroppedItem = Cast<AItem>(Item);
	AItem* SpawnedItem = GetWorld()->SpawnActor<AItem>(DroppedItem->ItemData.ItemClass, Location, FRotator(0, 0, 0));
	SpawnedItem->ItemData = InventoryItems[SlotIndex];
}

void URInventoryComponent::Server_DropInventoryItem_Implementation(int SlotIndex)
{
	DropInventoryItem(SlotIndex);
}

void URInventoryComponent::NetMulticast_DropItem_Implementation(int SlotIndex, AActor* Item, FVector Location)
{
	AItem* DroppedItem = Cast<AItem>(Item);
	if(DroppedItem && Item)
	{
		DroppedItem->ItemData = InventoryItems[SlotIndex];
		Item->SetActorLocation(Location);
		Item->SetActorHiddenInGame(false);
		Item->SetActorEnableCollision(true);
	}
}

void URInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicates to everyone
	DOREPLIFETIME(URInventoryComponent, InventoryItems);
}
