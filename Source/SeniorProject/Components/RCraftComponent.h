// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SeniorProject/Structs/FRecipeOfItem.h"
#include "RCraftComponent.generated.h"


class AItem;
struct FRecipeOfItem;
struct FItemData;
class URInventoryComponent;
class ASeniorProjectCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SENIORPROJECT_API URCraftComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URCraftComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable)
	int FindAmountOfRequiredItem(FItemData RequiredItemData);
	
	UFUNCTION(BlueprintCallable)
	void CraftItem(UPARAM(ref) const FRecipeOfItem& RecipeOfItem);

	UFUNCTION(BlueprintCallable)
	AItem* GetDefaultsOfClass(TSubclassOf<AItem> ItemClass) {return ItemClass.GetDefaultObject(); }
	
	UFUNCTION(Server, Reliable)
	void Server_CraftItem(const FRecipeOfItem& RecipeOfItem);

private:
	UPROPERTY()
	ASeniorProjectCharacter* Player;

	UPROPERTY()
	AItem* CraftedItem;

protected:
	UPROPERTY(BlueprintReadWrite)
	URInventoryComponent* InventoryComp;
};
