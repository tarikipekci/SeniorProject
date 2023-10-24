// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickups.generated.h"

UENUM(BlueprintType)
enum class EPickupItemType : uint8
{
	EDrinkable UMETA(DisplayName = "Drinkable"),
	EEdible UMETA(DisplayName = "Edible"),
	EHealth UMETA(DisplayName = "Health")
};

class ASeniorProjectCharacter;

UCLASS()
class SENIORPROJECT_API APickups : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickups();

protected:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere)
	float DecreaseAmount;

	UPROPERTY(EditAnywhere, Category = "Enums")
	EPickupItemType PickupType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(NetMulticast, Reliable)
	void DestroyActor();

public:
	void UseItem(class ASeniorProjectCharacter* Player);
};
