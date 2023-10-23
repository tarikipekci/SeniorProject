// Fill out your copyright notice in the Description page of Project Settings.


#include "RPlayerStatComponent.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

// Sets default values for this component's properties
URPlayerStatComponent::URPlayerStatComponent()
{
	MinHunger = 100.0f;
	MinThirst = 100.0f;
	MaxStamina = 100.0f;

	Hunger = MinHunger;
	Thirst = MinThirst;

	HungerAndThirstTimerDuration = 3.0f;
	StaminaRegenerationTimerDuration = 0.5f;
	HungerDecrementValue = 0.3;
	ThirstDecrementValue = 0.5f;
	StaminaDecrementValue = 3.5f;
	StaminaIncrementValue = 3.5f;
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
		LowerHunger(HungerDecrementValue);
		LowerThirst(ThirstDecrementValue);
	}
}

void URPlayerStatComponent::LowerHunger(float Value)
{
	if(GetOwnerRole() < ROLE_Authority)
	{
		ServerLowerHunger(Value);
	}
	else if(GetOwnerRole() == ROLE_Authority)
	{
		Hunger -= Value;
	}
}

void URPlayerStatComponent::LowerThirst(float Value)
{
	if(GetOwnerRole() < ROLE_Authority)
	{
		ServerLowerThirst(Value);
	}
	else if(GetOwnerRole() == ROLE_Authority)
	{
		Thirst -= Value;
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

bool URPlayerStatComponent::ServerLowerHunger_Validate(float Value)
{
	return true;
}

void URPlayerStatComponent::ServerLowerHunger_Implementation(float Value)
{
	if(GetOwnerRole() == ROLE_Authority)
	{
		LowerHunger(Value);
	}
}

bool URPlayerStatComponent::ServerLowerThirst_Validate(float Value)
{
	return true;
}

void URPlayerStatComponent::ServerLowerThirst_Implementation(float Value)
{
	if(GetOwnerRole() == ROLE_Authority)
	{
		LowerThirst(Value);
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
