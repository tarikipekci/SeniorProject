// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableInterface.h"
#include "GameFramework/Actor.h"
#include "SeniorProject/Structs/FItemData.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EPickupItemType : uint8
{
	EDrinkable UMETA(DisplayName = "Drinkable"),
	EEdible UMETA(DisplayName = "Edible"),
	EHealth UMETA(DisplayName = "Health")
};

class ASeniorProjectCharacter;

UCLASS()
class SENIORPROJECT_API AItem : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();

protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY(EditAnywhere)
	float ChangeAmount;

	UPROPERTY(EditAnywhere, Category = "Enums")
	EPickupItemType PickupType;
	
	UPROPERTY(EditDefaultsOnly)
	FItemData ItemData;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void InInventory(bool In);
	virtual void Interact(ASeniorProjectCharacter* Player) override;
	virtual void Use(ASeniorProjectCharacter* Player) override;
	FItemData GetItemData() const {return ItemData;}
};
