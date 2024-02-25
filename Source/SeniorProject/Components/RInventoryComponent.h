// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SeniorProject/Structs/FItemData.h"
#include "RInventoryComponent.generated.h"


class ASeniorProjectCharacter;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SENIORPROJECT_API URInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URInventoryComponent();

protected:
	UPROPERTY(ReplicatedUsing= OnRep_InventoryUpdated, BlueprintReadWrite, Category="Inventory")
	TArray<FItemData> InventoryItems;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	bool AddItem(FItemData ItemData);

	UFUNCTION(BlueprintCallable)
	void SwapItems(int index1, int index2);

	UFUNCTION()
	void OnRep_InventoryUpdated();

	UFUNCTION(BlueprintCallable)
	void DecreaseItemAmount(FItemData ItemData, int SlotIndex);

	UFUNCTION(BlueprintCallable)
	void RemoveItem(int SlotIndex);
	
	UFUNCTION(BlueprintCallable)
	const TArray<FItemData>& GetInventoryItems() const {return InventoryItems;}

	UFUNCTION(BlueprintCallable)
	void SetInventoryItems(TArray<FItemData> NewInventoryItems)
	{
		InventoryItems = NewInventoryItems;
	}

	UFUNCTION()
	void InitializeInventory();
	
private:
	UPROPERTY()
	ASeniorProjectCharacter* Player;

	UPROPERTY(Replicated)
	bool bIsNewItem;

	UPROPERTY()
	int itemIndex;
};
