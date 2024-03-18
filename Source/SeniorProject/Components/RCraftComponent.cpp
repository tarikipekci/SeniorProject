// Fill out your copyright notice in the Description page of Project Settings.


#include "RCraftComponent.h"

#include "RInventoryComponent.h"
#include "SeniorProject/Characters/SeniorProjectCharacter.h"
#include "SeniorProject/Environment/Item.h"
#include "SeniorProject/Structs/FRecipeOfItem.h"

// Sets default values for this component's properties
URCraftComponent::URCraftComponent()
{
}


// Called when the game starts
void URCraftComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Player = Cast<ASeniorProjectCharacter>(GetOwner());
	if(Player)
	{
		InventoryComp = Player->InventoryComp;
	}
}

int URCraftComponent::FindAmountOfRequiredItem(FItemData RequiredItemData)
{
	int CurrentItemAmount = 0;
	if(!RequiredItemData.ItemClass)
	{
		return 0;
	}

	for(auto ItemData : InventoryComp->GetInventoryItems())
	{
		if(ItemData.ItemClass == RequiredItemData.ItemClass)
		{
			CurrentItemAmount += ItemData.CurrentStackCount;
		}
	}
	return CurrentItemAmount;
}

void URCraftComponent::CraftItem(FRecipeOfItem RecipeOfItem)
{
	for(auto RequiredItem : RecipeOfItem.RequiredItems)
	{
		TSubclassOf<AItem> CurrentItemClass = RequiredItem.Key;
		AItem* CurrentItem = CurrentItemClass.GetDefaultObject();
		FItemData CurrentItemData = CurrentItem->ItemData;
		int RequiredAmount = RequiredItem.Value;
		for(FItemData& ItemData : InventoryComp->InventoryItems)
		{
			if(ItemData.ItemClass == CurrentItemData.ItemClass)
			{
				if(RequiredAmount <= ItemData.CurrentStackCount)
				{
					ItemData.CurrentStackCount -= RequiredAmount;

					if(ItemData.CurrentStackCount <= 0)
					{
						ItemData = FItemData();
					}
					break;
				}
				RequiredAmount -= ItemData.CurrentStackCount;
				ItemData = FItemData();
			}
		}
	}
	FItemData CraftedItem = RecipeOfItem.CraftedItem.GetDefaultObject()->ItemData;
	InventoryComp->AddItem(CraftedItem);
}
