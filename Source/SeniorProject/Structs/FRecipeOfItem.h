#pragma once
#include "FItemData.h"

#include "FRecipeOfItem.generated.h"

USTRUCT(BlueprintType)
struct FRecipeOfItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TSubclassOf<AItem>,int> RequiredItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AItem> CraftedItem;
};
