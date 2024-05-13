// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SeniorProject/Structs/FItemData.h"
#include "RInventoryComponent.generated.h"


class URCraftComponent;
class ASeniorProjectCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryUpdated, const TArray<FItemData>&, InventoryItems);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SENIORPROJECT_API URInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
	friend URCraftComponent;

public:
	// Sets default values for this component's properties
	URInventoryComponent();

	//Delegates
	UPROPERTY(BlueprintAssignable)
	FInventoryUpdated InventoryUpdated;

protected:
	UPROPERTY(ReplicatedUsing= OnRep_InventoryUpdated, BlueprintReadWrite, Category = "Inventory")
	TArray<FItemData> InventoryItems;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	bool AddItem(FItemData ItemData);

	UFUNCTION(Server, Reliable)
	void Server_AddItem(FItemData ItemData);

	UFUNCTION(BlueprintCallable)
	void SwapItems(URInventoryComponent* TargetInventory, int Index1, int Index2);

	UFUNCTION(Server, Reliable)
	void Server_SwapItems(URInventoryComponent* TargetInventory, int Index1, int Index2);

	UFUNCTION(BlueprintCallable)
	void OnRep_InventoryUpdated();

	UFUNCTION(BlueprintCallable)
	void DecreaseItemAmount(int SlotIndex, int Amount);

	UFUNCTION(Server, Reliable)
	void Server_DecreaseItemAmount(int SlotIndex, int Amount);

	UFUNCTION(BlueprintCallable)
	const TArray<FItemData>& GetInventoryItems() const { return InventoryItems; }

	UFUNCTION(BlueprintCallable)
	int GetInventoryMaxSlotSize() const {return InventoryMaxSlotSize;}

	UFUNCTION()
	void InitializeInventory();

	UFUNCTION(BlueprintCallable)
	void UseInventoryItem(int SlotIndex);

	UFUNCTION(Server, Reliable)
	void Server_UseInventoryItem(int SlotIndex);

	UFUNCTION(BlueprintCallable)
	void DropInventoryItem(int SlotIndex);

	UFUNCTION(Server, Reliable)
	void Server_DropInventoryItem(int SlotIndex);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_DropItem(int SlotIndex, AItem* Item, FVector Location);

	UFUNCTION(BlueprintCallable)
	AItem* GetEquippedItem() { return EquippedItem; }

	UFUNCTION(BlueprintCallable)
	void SetEquippedItem(AItem* Item) { EquippedItem = Item; }

	UFUNCTION(BlueprintCallable)
	void ChangeItemFromInventory(UPARAM(ref) FItemData& OldItemData, UPARAM(ref) FItemData& NewItemData);

	UFUNCTION(Server,Reliable)
	void Server_ChangeItemFromInventory(FItemData OldItemData, FItemData NewItemData);

private:
	UPROPERTY()
	ASeniorProjectCharacter* Player;

	UPROPERTY()
	int InventoryMaxSlotSize;

	UPROPERTY()
	int DropRange;

	UPROPERTY()
	int DropDepth;

	UPROPERTY(Replicated)
	AItem* EquippedItem;
};
