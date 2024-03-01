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
	//Hunger Properties
	UPROPERTY()
	float MaxHunger;
	float DefaultHunger;

	UPROPERTY(Replicated)
	float Hunger;

	UPROPERTY(EditAnywhere, Category = "S|PlayerStats")
	float HungerDecrementValue;


	//Thirst Properties
	UPROPERTY()
	float MaxThirst;
	float DefaultThirst;

	UPROPERTY(Replicated)
	float Thirst;

	UPROPERTY(EditAnywhere, Category = "S|PlayerStats")
	float ThirstDecrementValue;

	//Health Properties
	UPROPERTY(Replicated)
	float Health;
	float DamageMultiplier; //In case of hunger/thirst == 0

	UPROPERTY(EditAnywhere)
	float MaxHealth;

	//Stamina Properties
	UPROPERTY(Replicated)
	float Stamina;

	UPROPERTY()
	float MaxStamina;

	UPROPERTY()
	float StaminaRegenerationTimerDuration;

	UPROPERTY(Replicated)
	float HungerAndThirstTimerDuration;

	UPROPERTY(EditAnywhere, Category = "S|PlayerStats")
	float StaminaDecrementValue;

	UPROPERTY(EditAnywhere, Category = "S|PlayerStats")
	float StaminaIncrementValue;


	//Timers
	FTimerHandle TimerHandle; //For Hunger/Thirst
	FTimerHandle StaminaHandle; //For Stamina


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void HandleHungerAndThirst();

	//Server function of IncreaseHunger
	UFUNCTION(Server, Reliable)
	void ServerIncreaseHunger(float Value);

	//Server function of IncreaseThirst
	UFUNCTION(Server, Reliable)
	void ServerIncreaseThirst(float Value);

	//Server function of DecreaseStamina
	UFUNCTION(Server, Reliable)
	void ServerDecreaseStamina(float Value);

	//Server function of ControlSprintingTimer
	UFUNCTION(Server, Reliable)
	void ServerControlSprintingTimer(bool IsSprinting);

	//Server function of DecreaseHealth
	UFUNCTION(Server, Reliable)
	void ServerDecreaseHealth(float Value);

public:
	void ControlSprintingTimer(bool IsSprinting);

	//Increase Stats
	void IncreaseHunger(float Value);
	void IncreaseThirst(float Value);
	void IncreaseHealth(float Value);
	void RegenerateStamina();
	
	//Decrease Stats
	void DecreaseStamina(float Value);
	void DecreaseHunger(float Value);
	void DecreaseThirst(float Value);
	void DecreaseHealth(float Value);

	//Get Hunger Stats
	UFUNCTION(BlueprintCallable)
	float GetHunger() const { return Hunger; }

	UFUNCTION(BlueprintCallable)
	float GetMaxHunger() const { return MaxHunger; }

	UFUNCTION(BlueprintCallable)
	float GetDefaultHunger() const { return DefaultHunger; }

	//Get Thirst Stats
	UFUNCTION(BlueprintCallable)
	float GetThirst() const { return Thirst; }

	UFUNCTION(BlueprintCallable)
	float GetMaxThirst() const { return MaxThirst; }

	UFUNCTION(BlueprintCallable)
	float GetDefaultThirst() const { return DefaultThirst; }

	//Get Stamina Stats
	UFUNCTION(BlueprintCallable)
	float GetMaxStamina() const { return MaxStamina; }

	UFUNCTION(BlueprintCallable)
	float GetStamina() const { return Stamina; }

	UFUNCTION(BlueprintCallable)
	float GetStaminaDecrementValue() const { return StaminaDecrementValue; }

	//Get Health Stats
	UFUNCTION(BlueprintCallable)
	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const { return MaxHealth; }
};
