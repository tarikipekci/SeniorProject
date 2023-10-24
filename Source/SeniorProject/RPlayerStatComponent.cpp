// Fill out your copyright notice in the Description page of Project Settings.


#include "RPlayerStatComponent.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

// Sets default values for this component's properties
URPlayerStatComponent::URPlayerStatComponent()
{
	DefaultHunger = 0.0f;
	DefaultThirst = 0.0f;
	MaxHunger = 100.0f;
	MaxThirst = 100.0f;
	MaxStamina = 100.0f;

	Hunger = DefaultHunger;
	Thirst = DefaultThirst;

	HungerAndThirstTimerDuration = 3.0f;
	StaminaRegenerationTimerDuration = 0.5f;
	HungerDecrementValue = 0.3;
	ThirstDecrementValue = 0.5f;
	StaminaDecrementValue = 2.5f;
	StaminaIncrementValue = 3.0f;
	Stamina = MaxStamina;
}


// Called when the game starts
void URPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &URPlayerStatComponent::HandleHungerAndThirst,
	                                       HungerAndThirstTimerDuration, true);

	GetWorld()->GetTimerManager().SetTimer(StaminaHandle, this, &URPlayerStatComponent::RegenerateStamina,
	                                       StaminaRegenerationTimerDuration, true); // Regenerates stamina
}

void URPlayerStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicate to everyone
	DOREPLIFETIME(URPlayerStatComponent, Hunger);
	DOREPLIFETIME(URPlayerStatComponent, Thirst);
	DOREPLIFETIME(URPlayerStatComponent, Stamina);
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

void URPlayerStatComponent::LowerStamina(float Value)
{
	if(GetOwnerRole() < ROLE_Authority)
	{
		ServerLowerStamina(Value);
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

bool URPlayerStatComponent::ServerLowerStamina_Validate(float Value)
{
	return true;
}

void URPlayerStatComponent::ServerLowerStamina_Implementation(float Value)
{
	if(GetOwnerRole() == ROLE_Authority)
	{
		LowerStamina(Value);
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
