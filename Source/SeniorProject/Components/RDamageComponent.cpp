// Fill out your copyright notice in the Description page of Project Settings.


#include "RDamageComponent.h"

#include "RLifeComponent.h"
#include "Components/CapsuleComponent.h"
#include "SeniorProject/Environment/Item.h"
#include "SeniorProject/Notifies/AttackAnimNotifyState.h"

// Sets default values for this component's properties
URDamageComponent::URDamageComponent()
{
}


// Called when the game starts
void URDamageComponent::BeginPlay()
{
	Super::BeginPlay();

	InitAnimations();
	bIsAnimPlayed = false;
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
			if(OverlappedActor->FindComponentByClass<URLifeComponent>())
			{
				URLifeComponent* LifeComp = Cast<URLifeComponent>(OverlappedActor->FindComponentByClass<URLifeComponent>());
				if(DamageType == LifeComp->GetEffectiveDamageType())
				{
					AItem* UsedTool = Cast<AItem>(GetOwner());
					ASeniorProjectCharacter* DamagingPlayer = UsedTool->ItemData.PlayerEquipping;
					LifeComp->SetDamagingPlayer(DamagingPlayer);
					LifeComp->DecreaseHitPoints(DamageAmount);
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
