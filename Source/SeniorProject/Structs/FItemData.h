#pragma once

#include "FItemData.generated.h"

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class AItem> ItemClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTexture2D* ItemImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(BlueprintReadWrite)
	int CurrentStackCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int MaxStackSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bIsFull = false;
};

