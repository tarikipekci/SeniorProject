// Fill out your copyright notice in the Description page of Project Settings.


#include "RPlayerStatComponent.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

// Sets default values for this component's properties
URPlayerStatComponent::URPlayerStatComponent()
{
	HungerAndThirstTimerDuration = 3.0f;

	//Hunger
	DefaultHunger = 0.0f;
	MaxHunger = 100.0f;
	Hunger = DefaultHunger;
	HungerDecrementValue = 0.3;

	//Thirst
	DefaultThirst = 0.0f;
	MaxThirst = 100.0f;
	Thirst = DefaultThirst;
	ThirstDecrementValue = 0.5f;

	//Stamina
	MaxStamina = 100.0f;
	Stamina = MaxStamina;
	StaminaRegenerationTimerDuration = 0.5f;
	StaminaDecrementValue = 2.5f;
	StaminaIncrementValue = 3.0f;

	//Health
	MaxHealth = 100.0f;
	Health = MaxHealth;
}


// Called when the game starts
void URPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwnerRole() == ROLE_Authority)
	{
		SetIsReplicated(true);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &URPlayerStatComponent::HandleHungerAndThirst,
		                                       HungerAndThirstTimerDuration, true);

		GetWorld()->GetTimerManager().SetTimer(StaminaHandle, this, &URPlayerStatComponent::RegenerateStamina,
		                                       StaminaRegenerationTimerDuration, true); // Regenerates stamina
	}
}

void URPlayerStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicates to everyone
	DOREPLIFETIME(URPlayerStatComponent, Hunger);
	DOREPLIFETIME(URPlayerStatComponent, Thirst);
	DOREPLIFETIME(URPlayerStatComponent, Stamina);
	DOREPLIFETIME(URPlayerStatComponent, Health);
}

void URPlayerStatComponent::HandleHungerAndThirst()
{
	if(GetOwnerRole() == ROLE_Authority)
	{
		IncreaseHunger(HungerDecrementValue);
		IncreaseThirst(ThirstDecrementValue);
	}
}

void URPlayerStatComponent::IncreaseHunger(float Value)
{
	if(GetOwnerRole() < ROLE_Authority)
	{
		ServerIncreaseHunger(Value);
	}
	else if(GetOwnerRole() == ROLE_Authority)
	{
		Hunger += Value;
	}
}

void URPlayerStatComponent::IncreaseThirst(float Value)
{
	if(GetOwnerRole() < ROLE_Authority)
	{
		ServerIncreaseThirst(Value);
	}
	else if(GetOwnerRole() == ROLE_Authority)
	{
		Thirst += Value;
	}
}

void URPlayerStatComponent::DecreaseStamina(float Value)
{
	if(GetOwnerRole() < ROLE_Authority)
	{
		ServerDecreaseStamina(Value);
	}
	else if(GetOwnerRole() == ROLE_Authority)
	{
		if(Stamina - Value < 0.0f)
		{
			Stamina = 0.0f;
		}
		else
		{
			Stamina -= Value;
		}
	}
}

void URPlayerStatComponent::DecreaseHunger(float Value)
{
	if(GetOwnerRole() == ROLE_Authority)
	{
		if(Hunger - Value < 0)
		{
			Hunger = DefaultHunger;
		}
		else
		{
			Hunger -= Value;
		}
	}
}

void URPlayerStatComponent::DecreaseThirst(float Value)
{
	if(GetOwnerRole() == ROLE_Authority)
	{
		if(Thirst - Value < 0)
		{
			Thirst = DefaultThirst;
		}
		else
		{
			Thirst -= Value;
		}
	}
}

bool URPlayerStatComponent::ServerIncreaseHunger_Validate(float Value)
{
	return true;
}

void URPlayerStatComponent::ServerIncreaseHunger_Implementation(float Value)
{
	if(GetOwnerRole() == ROLE_Authority)
	{
		IncreaseHunger(Value);
	}
}

bool URPlayerStatComponent::ServerIncreaseThirst_Validate(float Value)
{
	return true;
}

void URPlayerStatComponent::ServerIncreaseThirst_Implementation(float Value)
{
	if(GetOwnerRole() == ROLE_Authority)
	{
		IncreaseThirst(Value);
	}
}

bool URPlayerStatComponent::ServerDecreaseStamina_Validate(float Value)
{
	return true;
}

void URPlayerStatComponent::ServerDecreaseStamina_Implementation(float Value)
{
	if(GetOwnerRole() == ROLE_Authority)
	{
		DecreaseStamina(Value);
	}
}

void URPlayerStatComponent::RegenerateStamina()
{
	if(GetOwnerRole() == ROLE_Authority)
	{
		if(Stamina >= MaxStamina)
		{
			Stamina = MaxStamina;
		}
		else
		{
			Stamina += 5.0f;
		}
	}
}

void URPlayerStatComponent::IncreaseHealth(float Value)
{
	if(GetOwnerRole() == ROLE_Authority)
	{
		if(Health + Value > MaxHealth)
		{
			Health = MaxHealth;
		}
		else
		{
			Health += Value;
		}
	}
}

void URPlayerStatComponent::ControlSprintingTimer(bool IsSprinting) //make it pause timer on the server
{
	if(GetOwnerRole() < ROLE_Authority)
	{
		ServerControlSprintingTimer(IsSprinting);
	}
	else if(GetOwnerRole() == ROLE_Authority)
	{
		if(IsSprinting)
		{
			GetWorld()->GetTimerManager().PauseTimer(StaminaHandle);
		}
		else
		{
			GetWorld()->GetTimerManager().UnPauseTimer(StaminaHandle);
		}
	}
}

bool URPlayerStatComponent::ServerControlSprintingTimer_Validate(bool IsSprinting)
{
	return true;
}

void URPlayerStatComponent::ServerControlSprintingTimer_Implementation(bool IsSprinting)
{
	if(GetOwnerRole() == ROLE_Authority)
	{
		ControlSprintingTimer(IsSprinting);
	}
}

void URPlayerStatComponent::DecreaseHealth(float Value)
{
	if(GetOwnerRole() < ROLE_Authority)
	{
		ServerDecreaseHealth(Value);
	}
	else if(GetOwnerRole() == ROLE_Authority)
	{
		Health -= Value;
	}
}

bool URPlayerStatComponent::ServerDecreaseHealth_Validate(float Value)
{
	return true;
}

void URPlayerStatComponent::ServerDecreaseHealth_Implementation(float Value)
{
	if(GetOwnerRole() == ROLE_Authority)
	{
		DecreaseHealth(Value);
	}
}
