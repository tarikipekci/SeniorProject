// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SeniorProject/Structs/FItemData.h"
#include "RInventoryComponent.generated.h"


class ASeniorProjectCharacter;

namespace EQSDebug
{
	struct FItemData;
}

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SENIORPROJECT_API URInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URInventoryComponent();

protected:
	UPROPERTY(ReplicatedUsing= OnRep_ItemPickedUp, BlueprintReadWrite, Category="Inventory")
	TArray<FItemData> InventoryItems;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void AddItem(FItemData ItemData);

	UFUNCTION()
	void OnRep_ItemPickedUp();
	
	UFUNCTION(BlueprintCallable)
	TArray<FItemData> GetInventoryItems() const {return InventoryItems;};

private:
	ASeniorProjectCharacter* Player;
};
