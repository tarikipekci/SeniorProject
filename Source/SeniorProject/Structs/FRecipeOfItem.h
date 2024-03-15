#pragma once
#include "FItemData.h"

#include "FRecipeOfItem.generated.h"

USTRUCT(BlueprintType)
struct FRecipeOfItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<AItem>,int> RequiredItems;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AItem> CraftedItem;
};
