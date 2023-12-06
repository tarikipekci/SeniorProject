// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SeniorProject/Structs/FItemData.h"
#include "RInteractComponent.generated.h"

struct FItemData;
class UInteractableInterface;
class IInteractableInterface;
class ASeniorProjectCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractionFound, FItemData, ItemData);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SENIORPROJECT_API URInteractComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URInteractComponent();

	UPROPERTY(BlueprintAssignable)
	FInteractionFound InteractionFound;

	UFUNCTION(BlueprintCallable)
	float GetInteractRange() const { return InteractRange; }

	UFUNCTION(BlueprintCallable)
	bool GetIsInteracting() const { return bIsInteracting; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UInteractableInterface* PerformInteractCheck();

	UFUNCTION(BlueprintCallable)
	void Interact();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerPickup(const AActor* Actor);
	bool ServerPickup_Validate(const AActor* Actor);
	void ServerPickup_Implementation(const AActor* Actor);

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
	AActor* InteractedActor;
};
