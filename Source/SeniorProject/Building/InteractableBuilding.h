// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseBuilding.h"
#include "SeniorProject/Environment/InteractableInterface.h"
#include "InteractableBuilding.generated.h"

/**
 * 
 */
UCLASS()
class SENIORPROJECT_API AInteractableBuilding : public ABaseBuilding, public IInteractableInterface
{
	GENERATED_BODY()

public:
	virtual void Interact(ASeniorProjectCharacter* Player) override;

	UFUNCTION(BlueprintCallable)
	virtual void Use(ASeniorProjectCharacter* Player) override;
};
