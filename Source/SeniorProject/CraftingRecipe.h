// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Structs/FRecipeOfItem.h"
#include "CraftingRecipe.generated.h"

/**
 * 
 */
UCLASS()
class SENIORPROJECT_API UCraftingRecipe : public UDataAsset
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FRecipeOfItem RecipeOfItem;
};
