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
	UPROPERTY()
	float MaxHunger;

	UPROPERTY(Replicated)
	float Hunger;

	UPROPERTY(EditAnywhere, Category = "S|PlayerStats")
	float HungerDecrementValue;

	UPROPERTY()
	float MaxThirst;
	float DefaultHunger;
	float DefaultThirst;

	UPROPERTY(Replicated)
	float Thirst;

	UPROPERTY(Replicated)
	float Health;

	UPROPERTY(EditAnywhere)
	float MaxHealth;

	UPROPERTY(EditAnywhere, Category = "S|PlayerStats")
	float ThirstDecrementValue;

	UPROPERTY(Replicated)
	float HungerAndThirstTimerDuration;

	UPROPERTY(Replicated)
	float Stamina;

	UPROPERTY()
	float MaxStamina;

	UPROPERTY()
	float StaminaRegenerationTimerDuration;

	FTimerHandle TimerHandle;
	FTimerHandle StaminaHandle;

	UPROPERTY(EditAnywhere, Category = "S|PlayerStats")
	float StaminaDecrementValue;

	UPROPERTY(EditAnywhere, Category = "S|PlayerStats")
	float StaminaIncrementValue;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void HandleHungerAndThirst();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerIncreaseHunger(float Value);
	bool ServerIncreaseHunger_Validate(float Value);
	void ServerIncreaseHunger_Implementation(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerIncreaseThirst(float Value);
	bool ServerIncreaseThirst_Validate(float Value);
	void ServerIncreaseThirst_Implementation(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerLowerStamina(float Value);
	bool ServerLowerStamina_Validate(float Value);
	void ServerLowerStamina_Implementation(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerControlSprintingTimer(bool IsSprinting);
	bool ServerControlSprintingTimer_Validate(bool IsSprinting);
	void ServerControlSprintingTimer_Implementation(bool IsSprinting);

	void RegenerateStamina();

public:
	void IncreaseHunger(float Value);
	void IncreaseThirst(float Value);
	void IncreaseHealth(float Value);
	void LowerStamina(float Value);
	void DecreaseHunger(float Value);
	void DecreaseThirst(float Value);

	UFUNCTION(BlueprintCallable)
	float GetHunger() const { return Hunger; }

	UFUNCTION(BlueprintCallable)
	float GetMaxHunger() const { return MaxHunger; }

	UFUNCTION(BlueprintCallable)
	float GetThirst() const { return Thirst; }

	UFUNCTION(BlueprintCallable)
	float GetMaxThirst() const { return MaxThirst; }

	UFUNCTION(BlueprintCallable)
	float GetMaxStamina() const { return MaxStamina; }

	UFUNCTION(BlueprintCallable)
	float GetStamina() const { return Stamina; }

	UFUNCTION(BlueprintCallable)
	float GetDefaultHunger() const { return DefaultHunger; }

	UFUNCTION(BlueprintCallable)
	float GetDefaultThirst() const { return DefaultThirst; }

	UFUNCTION(BlueprintCallable)
	float GetStaminaDecrementValue() const { return StaminaDecrementValue; }

	UFUNCTION(BlueprintCallable)
	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const { return MaxHealth; }

	void ControlSprintingTimer(bool IsSprinting);
};
