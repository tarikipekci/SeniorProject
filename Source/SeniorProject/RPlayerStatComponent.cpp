// Fill out your copyright notice in the Description page of Project Settings.


#include "RPlayerStatComponent.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

// Sets default values for this component's properties
URPlayerStatComponent::URPlayerStatComponent()
{
	MinHunger = 100.0f;
	MinThirst = 100.0f;

	Hunger = MinHunger;
	Thirst = MinThirst;

	HungerAndThirstTimerDuration = 3.0f;
	HungerDecrementValue = 0.3;
	ThirstDecrementValue = 0.5f;
}


// Called when the game starts
void URPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &URPlayerStatComponent::HandleHungerAndThirst,
	                                       HungerAndThirstTimerDuration, true);
}

void URPlayerStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicate to everyone
	DOREPLIFETIME(URPlayerStatComponent, Hunger);
	DOREPLIFETIME(URPlayerStatComponent, Thirst);
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
