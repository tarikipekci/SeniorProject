// Fill out your copyright notice in the Description page of Project Settings.


#include "RDamageComponent.h"

#include "RLifeComponent.h"
#include "Components/CapsuleComponent.h"
#include "SeniorProject/AttackAnimNotifyState.h"
#include "SeniorProject/Environment/Item.h"
#include "SeniorProject/Environment/LootableActor.h"

// Sets default values for this component's properties
URDamageComponent::URDamageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URDamageComponent::BeginPlay()
{
	Super::BeginPlay();

	InitAnimations();
	bIsAnimPlayed = false;
}


// Called every frame
void URDamageComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URDamageComponent::InitAnimations()
{
	if(PlayerAttackMontage)
	{
		const auto NotifyEvents = PlayerAttackMontage->Notifies;
		for(auto EventNotify : NotifyEvents)
		{
			if(const auto AttackNotify = Cast<UAttackAnimNotifyState>(EventNotify.NotifyStateClass))
			{
				AttackNotify->OnNotifyBegin.AddDynamic(this, &URDamageComponent::OnAnimationBegin);
				AttackNotify->OnNotifyTick.AddDynamic(this, &URDamageComponent::OnAnimation);
				AttackNotify->OnNotifyEnd.AddDynamic(this, &URDamageComponent::OnAnimationFinish);
			}
		}
	}
}

void URDamageComponent::OnAnimationBegin()
{
	AItem* OwnerItem = Cast<AItem>(GetOwner());
	if(OwnerItem)
	{
		if(OwnerItem->bIsEquipped)
		{
			OwnerItem->SetActorEnableCollision(true);
			OwnerItem->InteractCollision->SetCollisionProfileName("OverlapAll");
		}
	}
}

void URDamageComponent::OnAnimation()
{
	if(bIsAnimPlayed == false)
	{
		TArray<AActor*> OverlappingActors;
		Cast<AItem>(GetOwner())->InteractCollision->GetOverlappingActors(OverlappingActors);

		for(auto OverlappedActor : OverlappingActors)
		{
			if(OverlappedActor->GetClass()->IsChildOf(ALootableActor::StaticClass()))
			{
				ALootableActor* LootableActor = Cast<ALootableActor>(OverlappedActor);
				if(DamageType == LootableActor->GetEffectiveDamageType())
				{
					LootableActor->GetLifeComponent()->DecreaseHitPoints(DamageAmount);
				}
				bIsAnimPlayed = true;
				break;
			}
		}
	}
}

void URDamageComponent::OnAnimationFinish()
{
	bIsAnimPlayed = false;

	AItem* OwnerItem = Cast<AItem>(GetOwner());
	if(OwnerItem)
	{
		if(OwnerItem->bIsEquipped)
		{
			OwnerItem->SetActorEnableCollision(false);
		}
	}
}
