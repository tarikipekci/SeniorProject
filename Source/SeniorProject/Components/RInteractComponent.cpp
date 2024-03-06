// Fill out your copyright notice in the Description page of Project Settings.


#include "RInteractComponent.h"

#include "RInventoryComponent.h"
#include "RLineTraceComponent.h"
#include "Camera/CameraComponent.h"
#include "SeniorProject/Building/InventoryBuilding.h"
#include "SeniorProject/Environment/InteractableInterface.h"
#include "SeniorProject/Environment/Item.h"

// Sets default values for this component's properties
URInteractComponent::URInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	InteractRange = 200.0f;
}


// Called when the game starts
void URInteractComponent::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<ASeniorProjectCharacter>(GetOwner());
	checkf(Player, TEXT(""));
}


void URInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	PerformInteractCheck();
}

bool URInteractComponent::PerformInteractCheck()
{
	FVector Start = Player->InteractComp->GetComponentLocation();
	FVector End = Start + Player->GetFollowCamera()->GetForwardVector() * InteractRange;
	AActor* Actor = Player->LineTraceComp->LineTraceSingle(Start, End, INTERACTABLE_CHANNEL, false);
	if(Actor)
	{
		if(Actor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
		{
			UInteractableInterface* Interface = Cast<UInteractableInterface>(DetectedInteractableActor);
			DetectedInteractableActor = Actor;

			if(AItem* Item = Cast<AItem>(DetectedInteractableActor))
			{
				PickUpFound.Broadcast(Item);
			}
			else if(AInventoryBuilding* InventoryBuilding = Cast<AInventoryBuilding>(DetectedInteractableActor))
			{
				InventoryBuildingFound.Broadcast(InventoryBuilding);
			}
			bIsInteracting = true;
			return bIsInteracting;
		}
	}
	bIsInteracting = false;
	return bIsInteracting;
}

void URInteractComponent::Interact()
{
	if(DetectedInteractableActor && bIsInteracting)
	{
		Server_Interact(DetectedInteractableActor);
	}
}

void URInteractComponent::Server_Interact_Implementation(AActor* Actor)
{
	if(Player->HasAuthority())
	{
		FVector Start = Player->InteractComp->GetComponentLocation();
		FVector End = Actor->GetActorLocation();
		AActor* HitActor = Player->LineTraceComp->LineTraceSingle(Start, End, INTERACTABLE_CHANNEL, false);
		if(Actor && HitActor)
		{
			if(HitActor == Actor)
			{
				if(HitActor->GetClass()->IsChildOf(AItem::StaticClass()))
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
				else if(HitActor->GetClass()->IsChildOf(AInventoryBuilding::StaticClass()))
				{
					AInventoryBuilding* InventoryBuilding = Cast<AInventoryBuilding>(HitActor);
					if(InventoryBuilding)
					{
						if(IInteractableInterface* Interface = Cast<IInteractableInterface>(InventoryBuilding))
						{
							InventoryBuilding->Interact(Player);
						}
					}
				}
			}
		}
		InteractedActor = DetectedInteractableActor;
	}
}
