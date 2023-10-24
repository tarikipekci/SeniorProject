// Fill out your copyright notice in the Description page of Project Settings.


#include "RLineTraceComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
URLineTraceComponent::URLineTraceComponent()
{
}


// Called when the game starts
void URLineTraceComponent::BeginPlay()
{
	Super::BeginPlay();
}

AActor* URLineTraceComponent::LineTraceSingle(FVector Start, FVector End)
{
	FHitResult HitResult;
	FCollisionObjectQueryParams CollisionParams;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());
	if(GetWorld()->LineTraceSingleByObjectType(OUT HitResult, Start, End, CollisionParams, CollisionQueryParams))
	{
		return HitResult.GetActor();
	}

	return nullptr;
}

AActor* URLineTraceComponent::LineTraceSingle(FVector Start, FVector End, bool ShowDebugLine)
{
	AActor* Actor = LineTraceSingle(Start, End);
	if(ShowDebugLine)
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 3.0f, 0.0f, 5.0f);
	}
	return Actor;
}
