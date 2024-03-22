// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableInterface.h"
#include "GameFramework/Actor.h"
#include "SeniorProject/Structs/FItemData.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None UMETA(DisplayName = "None"),
	Material UMETA(DisplayerName = "Material"),
	Tool UMETA(DisplayName = "Tool"),
	Drinkable UMETA(DisplayName = "Drinkable"),
	Edible UMETA(DisplayName = "Edible"),
	Health UMETA(DisplayName = "Health")
};

class ASeniorProjectCharacter;

UCLASS()
class SENIORPROJECT_API AItem : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();

public:
	UPROPERTY(EditAnywhere, Category= "ItemInfo")
	FItemData ItemData;

	UPROPERTY(BlueprintReadWrite, Replicated)
	bool bIsInteractable;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY(EditAnywhere)
	float ChangeAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enums")
	EItemType ItemType;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Interact(ASeniorProjectCharacter* Player) override;

	UFUNCTION(BlueprintCallable)
	virtual void Use(ASeniorProjectCharacter* Player) override;

	UFUNCTION(BlueprintCallable)
	FItemData GetItemData() const {return ItemData;}

	UFUNCTION(NetMulticast, Reliable)
	void DestroyItem();
};
