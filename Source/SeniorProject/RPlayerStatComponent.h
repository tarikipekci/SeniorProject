// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPlayerStatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SENIORPROJECT_API URPlayerStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URPlayerStatComponent();

protected:
	UPROPERTY(Replicated)
	float MinHunger;

	UPROPERTY(Replicated)
	float Hunger;

	UPROPERTY(EditAnywhere, Category = "S|PlayerStats")
	float HungerDecrementValue;

	UPROPERTY(Replicated)
	float MinThirst;

	UPROPERTY(Replicated)
	float Thirst;

	UPROPERTY(EditAnywhere, Category = "S|PlayerStats")
	float ThirstDecrementValue;

	UPROPERTY(Replicated)
	float HungerAndThirstTimerDuration;

	FTimerHandle TimerHandle;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void HandleHungerAndThirst();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerLowerHunger(float Value);
	bool ServerLowerHunger_Validate(float Value);
	void ServerLowerHunger_Implementation(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerLowerThirst(float Value);
	bool ServerLowerThirst_Validate(float Value);
	void ServerLowerThirst_Implementation(float Value);

public:
	void LowerHunger(float Value);
	void LowerThirst(float Value);
	
	UFUNCTION(BlueprintCallable)
	float GetHunger() const {return Hunger;}
	
	UFUNCTION(BlueprintCallable)
	float GetMinHunger() const {return MinHunger;}

	UFUNCTION(BlueprintCallable)
	float GetThirst() const {return Thirst;}

	UFUNCTION(BlueprintCallable)
	float GetMinThirst() const {return MinThirst;}
};
