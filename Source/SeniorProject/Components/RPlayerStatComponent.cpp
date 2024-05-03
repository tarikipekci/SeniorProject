// Fill out your copyright notice in the Description page of Project Settings.


#include "RPlayerStatComponent.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "Engine/DamageEvents.h"
#include "SeniorProject/Characters/SeniorProjectCharacter.h"

// Sets default values for this component's properties
URPlayerStatComponent::URPlayerStatComponent()
{
	HungerAndThirstTimerDuration = 3.0f;

	//Hunger
	DefaultHunger = 0.0f;
	MaxHunger = 100.0f;
	Hunger = DefaultHunger;
	HungerDecrementValue = 0.3f;

	//Thirst
	DefaultThirst = 0.0f;
	MaxThirst = 100.0f;
	Thirst = DefaultThirst;
	ThirstDecrementValue = 0.6f;

	//Stamina
	MaxStamina = 100.0f;
	Stamina = MaxStamina;
	StaminaRegenerationTimerDuration = 0.5f;
	StaminaDecrementValue = 1.5f;
	StaminaIncrementValue = 2.5f;

	//Health
	MaxHealth = 100.0f;
	Health = MaxHealth;
	DamageMultiplier = 0.1f;
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
	DOREPLIFETIME_CONDITION(URPlayerStatComponent, Hunger, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(URPlayerStatComponent, Thirst, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(URPlayerStatComponent, Stamina, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(URPlayerStatComponent, Health, COND_OwnerOnly);
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

		if(Hunger >= MaxHunger)
		{
			if(ASeniorProjectCharacter* Character = Cast<ASeniorProjectCharacter>(GetOwner()))
			{
				Character->TakeDamage(Hunger * DamageMultiplier, FDamageEvent(), Character->GetController(), Character);
				Hunger = MaxHunger;
			}
		}
	}
}

void URPlayerStatComponent::ServerDecreaseHunger_Implementation(float Value)
{
	DecreaseHunger(Value);
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

		if(Thirst >= MaxThirst)
		{
			if(ASeniorProjectCharacter* Character = Cast<ASeniorProjectCharacter>(GetOwner()))
			{
				Character->TakeDamage(Thirst * DamageMultiplier, FDamageEvent(), Character->GetController(), Character);
				Thirst = MaxThirst;
			}
		}
	}
}

void URPlayerStatComponent::ServerDecreaseThirst_Implementation(float Value)
{
	DecreaseThirst(Value);
}

void URPlayerStatComponent::DecreaseStamina(float Value)
{
	if(GetOwnerRole() < ROLE_Authority)
	{
		ServerDecreaseStamina(Value);
	}
	else if(GetOwnerRole() == ROLE_Authority)
	{
		if(Stamina - Value <= 0.0f)
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
	if(GetOwner()->HasAuthority())
	{
		if(Hunger - Value <= 0)
		{
			Hunger = DefaultHunger;
		}
		else
		{
			Hunger -= Value;
		}
	}
	else
	{
		ServerDecreaseHunger(Value);
	}
}

void URPlayerStatComponent::DecreaseThirst(float Value)
{
	if(GetOwner()->HasAuthority())
	{
		if(Thirst - Value <= 0)
		{
			Thirst = DefaultThirst;
		}
		else
		{
			Thirst -= Value;
		}
	}
	else
	{
		ServerDecreaseThirst(Value);
	}
}

void URPlayerStatComponent::ServerIncreaseHunger_Implementation(float Value)
{
	if(GetOwnerRole() == ROLE_Authority)
	{
		IncreaseHunger(Value);
	}
}

void URPlayerStatComponent::ServerIncreaseThirst_Implementation(float Value)
{
	if(GetOwnerRole() == ROLE_Authority)
	{
		IncreaseThirst(Value);
	}
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

void URPlayerStatComponent::ServerDecreaseHealth_Implementation(float Value)
{
	if(GetOwnerRole() == ROLE_Authority)
	{
		DecreaseHealth(Value);
	}
}
