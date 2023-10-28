// Fill out your copyright notice in the Description page of Project Settings.


#include "RLineTraceComponent.h"

// Sets default values for this component's properties
URLineTraceComponent::URLineTraceComponent()
{
}


// Called when the game starts
void URLineTraceComponent::BeginPlay()
{
	Super::BeginPlay();
}

AActor* URLineTraceComponent::LineTraceSingle(FVector Start, FVector End, ECollisionChannel Channel, bool ShowDebugLine)
{
	Channel = INTERACTABLE_CHANNEL;
	FHitResult HitResult;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());

	if(GetWorld()->LineTraceSingleByChannel(OUT HitResult, Start, End, Channel, CollisionQueryParams))
	{
		return HitResult.GetActor();
	}

#ifdef DEBUG_ENABLED
	if(ShowDebugLine)
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 3.0f, 0.0f, 5.0f);
	}
	return HitResult.GetActor();
#endif
}
