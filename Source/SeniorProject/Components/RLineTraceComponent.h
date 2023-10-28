// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RLineTraceComponent.generated.h"

#define INTERACTABLE_CHANNEL ECC_GameTraceChannel1
#define DEBUG_ENABLED

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SENIORPROJECT_API URLineTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URLineTraceComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	AActor* LineTraceSingle(FVector Start, FVector End, ECollisionChannel Channel, bool ShowDebugLine = false);
};
