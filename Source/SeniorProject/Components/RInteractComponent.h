// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "RInteractComponent.generated.h"


class ASeniorProjectCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SENIORPROJECT_API URInteractComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URInteractComponent();

public:
	float GetInteractRange() const { return InteractRange; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void Pickup();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerPickup(AActor* Actor);
	bool ServerPickup_Validate(AActor* Actor);
	void ServerPickup_Implementation(AActor* Actor);

private:
	ASeniorProjectCharacter* Player;

	UPROPERTY()
	float InteractRange;
};
