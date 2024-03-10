// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "RInteractComponent.generated.h"

class AInventoryBuilding;
class AItem;
struct FItemData;
class UInteractableInterface;
class IInteractableInterface;
class ASeniorProjectCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPickUpFound, AItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryBuildingFound, AInventoryBuilding*, InventoryBuilding);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SENIORPROJECT_API URInteractComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URInteractComponent();

	//Delegates
	UPROPERTY(BlueprintAssignable)
	FPickUpFound PickUpFound;
	
	UPROPERTY(BlueprintAssignable)
	FInventoryBuildingFound InventoryBuildingFound;

	UFUNCTION(BlueprintCallable)
	float GetInteractRange() const { return InteractRange; }

	UFUNCTION(BlueprintCallable)
	bool GetIsInteracting() const { return bIsInteracting; }

	UFUNCTION(BlueprintCallable)
	AActor* GetInteractedActor() const {return InteractedActor;}

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	bool PerformInteractCheck();

	UFUNCTION(BlueprintCallable)
	void Interact();

	UFUNCTION(Server, Reliable)
	void Server_Interact(AActor* Actor);

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	ASeniorProjectCharacter* Player;

	UPROPERTY()
	float InteractRange;

	UPROPERTY()
	bool bIsInteracting;

	UPROPERTY()
	AActor* DetectedInteractableActor;

	UPROPERTY()
	AActor* InteractedActor;
};
