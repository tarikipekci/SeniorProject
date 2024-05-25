// Fill out your copyright notice in the Description page of Project Settings.


#include "RDamageComponent.h"

#include "RLifeComponent.h"
#include "RPlayerStatComponent.h"
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
		AItem* UsedTool = Cast<AItem>(GetOwner());

		if (UsedTool)
		{
			UsedTool->InteractCollision->GetOverlappingActors(OverlappingActors);

			ASeniorProjectCharacter* DamagingPlayer = UsedTool->ItemData.PlayerEquipping;

			if (DamagingPlayer)
			{
				for (auto OverlappedActor : OverlappingActors)
				{
					if (OverlappedActor == DamagingPlayer)
					{
						continue;
					}

					URLifeComponent* LifeComp = Cast<URLifeComponent>(OverlappedActor->FindComponentByClass<URLifeComponent>());
					if (LifeComp)
					{
						if (DamageType == LifeComp->GetEffectiveDamageType())
						{
							LifeComp->SetDamagingPlayer(DamagingPlayer);
							LifeComp->DecreaseHitPoints(DamageAmount);
							bIsAnimPlayed = true;
							break;
						}
					}
					else if (ASeniorProjectCharacter* Player = Cast<ASeniorProjectCharacter>(OverlappedActor))
					{
						if (Player != DamagingPlayer) 
						{
							Player->PlayerStatComp->DecreaseHealth(DamageAmount);
							bIsAnimPlayed = true;
							break;
						}
					}
				}
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
