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
	PrimaryComponentTick.bCanEverTick = true;
	InteractRange = 200.0f;
}


// Called when the game starts
void URInteractComponent::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<ASeniorProjectCharacter>(GetOwner());
	checkf(Player,TEXT(""));
}


void URInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	PerformInteractCheck();
}

UInteractableInterface* URInteractComponent::PerformInteractCheck()
{
	FVector Start = Player->InteractComp->GetComponentLocation();
	FVector End = Start + Player->GetFollowCamera()->GetForwardVector() * InteractRange;
	AActor* Actor = Player->LineTraceComp->LineTraceSingle(Start, End, INTERACTABLE_CHANNEL, false);
	if(Actor)
	{
		if(Actor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
		{
			UInteractableInterface* Interface = Cast<UInteractableInterface>(InteractedActor);
			InteractedActor = Actor;
			if(AItem* Item = Cast<AItem>(InteractedActor))
			{
				InteractionFound.Broadcast(Item->GetItemData());
			}
			bIsInteracting = true;
			return Interface;
		}
	}
	bIsInteracting = false;
	return nullptr;
}

void URInteractComponent::Interact()
{
	if(InteractedActor && bIsInteracting)
	{
		if(AItem* Item = Cast<AItem>(InteractedActor))
		{
			Server_Interact(Item);
		}
	}
}

void URInteractComponent::Server_Interact_Implementation(const AActor* Actor)
{
	if(Player->HasAuthority())
	{
		FVector Start = Player->InteractComp->GetComponentLocation();
		FVector End = Actor->GetActorLocation();
		AActor* HitActor = Player->LineTraceComp->LineTraceSingle(Start, End, INTERACTABLE_CHANNEL, false);
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
