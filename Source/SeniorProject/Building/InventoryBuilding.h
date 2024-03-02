// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableBuilding.h"
#include "InventoryBuilding.generated.h"

/**
 * 
 */
UCLASS()
class SENIORPROJECT_API AInventoryBuilding : public AInteractableBuilding
{
	GENERATED_BODY()
	AInventoryBuilding();

public:
	virtual void Interact(ASeniorProjectCharacter* Player) override;

protected:
	UPROPERTY(BlueprintReadWrite, Replicated)
	bool bOpened;
};
