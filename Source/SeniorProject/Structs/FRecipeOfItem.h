#pragma once
#include "FItemData.h"

#include "FRecipeOfItem.generated.h"

USTRUCT(BlueprintType)
struct FRecipeOfItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<AItem>> RequiredItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int> RequiredAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AItem> CraftedItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString RecipeDescription;
};
