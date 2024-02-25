// Fill out your copyright notice in the Description page of Project Settings.


#include "RInventoryComponent.h"

#include "Net/UnrealNetwork.h"
#include "../Environment/Item.h"

// Sets default values for this component's properties
URInventoryComponent::URInventoryComponent()
{
	itemIndex = 0;
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
	if(!ItemData.ItemClass)
	{
		return false;
	}

	bool bAdded = false;
	int EmptySlotIndex = -1;

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
			else
			{
				int RemainingCount = NeededSize - AvailableSize;
				Item.CurrentStackCount = Item.MaxStackSize;
				ItemData.CurrentStackCount = RemainingCount;
			}
		}
	}

	if(!bAdded && EmptySlotIndex >= 0)
	{
		InventoryItems[EmptySlotIndex] = ItemData;
		bAdded = true;
	}

	if(bAdded)
	{
		OnRep_InventoryUpdated();
	}
	return bAdded;
	
	/*return;
	if(Player->HasAuthority())
	{
		int TotalCountOfItemData = 0;
		int slotIndex = -1;
		bool emptySlotFound = false;
		TArray<FItemData> FullSlotsOfItemData;
		for(FItemData& Item : InventoryItems)
		{
			slotIndex++;
			if(!Item.ItemClass && emptySlotFound == false)
			{
				emptySlotFound = true;
				itemIndex = slotIndex;
			}
			if(Item.ItemClass == ItemData.ItemClass)
			{
				TotalCountOfItemData++;
				if(!Item.bIsFull)
				{
					Item.CurrentStackCount++;
					bIsNewItem = false;
					itemIndex = slotIndex;
					OnRep_ItemPickedUp(); //Updated existing item
					if(Item.CurrentStackCount >= Item.MaxStackSize)
					{
						Item.bIsFull = true;
					}
					break;
				}
				FullSlotsOfItemData.Add(Item);
			}
		}
		if(TotalCountOfItemData == FullSlotsOfItemData.Num())
		{
			bIsNewItem = true;
			ItemData.CurrentStackCount++;
			InventoryItems.Add(ItemData);
			OnRep_ItemPickedUp(); //Added new item
		}
	}*/
}

void URInventoryComponent::SwapItems(int index1, int index2)
{
	
}

void URInventoryComponent::OnRep_InventoryUpdated()
{
	if(Player)
	{
		Player->InventoryUpdated.Broadcast(InventoryItems);
	}
	/*if(InventoryItems.Num() > 0)
	{
		Player->ItemPickedUp.Broadcast(InventoryItems[InventoryItems.Num() - 1], InventoryItems, bIsNewItem,
		                               itemIndex);
	}*/
}

void URInventoryComponent::DecreaseItemAmount(FItemData ItemData, int SlotIndex)
{
	ItemData.CurrentStackCount--;
	if(ItemData.CurrentStackCount <= 0)
	{
		RemoveItem(SlotIndex);
	}
}

void URInventoryComponent::RemoveItem(int SlotIndex)
{
	InventoryItems.RemoveAt(SlotIndex);
}

void URInventoryComponent::InitializeInventory()
{
	InventoryItems.Reserve(12);

	for(int i = 0; i < 12; i++)
	{
		InventoryItems.Emplace();
	}
}

void URInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicates to everyone
	DOREPLIFETIME_CONDITION(URInventoryComponent, InventoryItems, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(URInventoryComponent, bIsNewItem, COND_OwnerOnly);
}
