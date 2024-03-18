// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Structs/FRecipeOfItem.h"
#include "CraftingRecipe.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class SENIORPROJECT_API UCraftingRecipe : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRecipeOfItem RecipeOfItem;
};
