// Fill out your copyright notice in the Description page of Project Settings.


#include "RInventoryComponent.h"

#include "Net/UnrealNetwork.h"
#include "..\Environment\Item.h"

// Sets default values for this component's properties
URInventoryComponent::URInventoryComponent()
{
}

// Called when the game starts
void URInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<ASeniorProjectCharacter>(GetOwner());
	SetIsReplicated(true);
}


void URInventoryComponent::AddItem(FItemData ItemData)
{
	if(Player->HasAuthority())
	{
		int TotalCountOfItemData = 0;
		TArray<FItemData> FullSlotsOfItemData;
		for(FItemData& Item : InventoryItems)
		{
			if(Item.ItemClass == ItemData.ItemClass)
			{
				TotalCountOfItemData++;
				if(!Item.bIsFull)
				{
					Item.CurrentStackCount++;
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
			ItemData.CurrentStackCount++;
			InventoryItems.Add(ItemData);
			OnRep_ItemPickedUp();
		}
		Player->ItemStackSizeUpdated.Broadcast(InventoryItems);
	}
}


void URInventoryComponent::OnRep_ItemPickedUp()
{
	if(InventoryItems.Num())
	{
		Player->ItemPickedUp.Broadcast(InventoryItems[InventoryItems.Num() - 1]);
	}
}

void URInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicates to everyone
	DOREPLIFETIME_CONDITION(URInventoryComponent, InventoryItems, COND_OwnerOnly);
}
