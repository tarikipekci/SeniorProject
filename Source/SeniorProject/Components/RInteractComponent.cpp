// Fill out your copyright notice in the Description page of Project Settings.


#include "RInteractComponent.h"

#include "RInventoryComponent.h"
#include "RLineTraceComponent.h"
#include "Camera/CameraComponent.h"
#include "SeniorProject/Environment/InteractableInterface.h"
#include "SeniorProject/Environment/Item.h"

// Sets default values for this component's properties
URInteractComponent::URInteractComponent()
{
	InteractRange = 200.0f;
}


// Called when the game starts
void URInteractComponent::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<ASeniorProjectCharacter>(GetOwner());
}

void URInteractComponent::Pickup()
{
	FVector Start = Player->InteractComp->GetComponentLocation();
	FVector End = Start + Player->GetFollowCamera()->GetForwardVector() * InteractRange;
	AActor* Actor = Player->LineTraceComp->LineTraceSingle(Start, End, INTERACTABLE_CHANNEL, true);
	if(Actor)
	{
		if(AItem* Item = Cast<AItem>(Actor))
		{
			ServerPickup(Item);
		}
	}
}

bool URInteractComponent::ServerPickup_Validate(AActor* Actor)
{
	return true;
}

void URInteractComponent::ServerPickup_Implementation(AActor* Actor)
{
	if(Player->HasAuthority())
	{
		FVector Start = Player->InteractComp->GetComponentLocation();
		FVector End = Actor->GetActorLocation();
		AActor* HitActor = Player->LineTraceComp->LineTraceSingle(Start, End, INTERACTABLE_CHANNEL, true);
		if(Actor)
		{
			if(HitActor == Actor)
			{
				AItem* Item = Cast<AItem>(HitActor);
				if(Item)
				{
					if(IInteractableInterface* Interface = Cast<IInteractableInterface>(Item))
					{
						Interface->Interact(Player);
					}
				}
			}
		}
	}
}
