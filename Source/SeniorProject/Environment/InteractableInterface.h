// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SeniorProject/Characters/SeniorProjectCharacter.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SENIORPROJECT_API IInteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Interact(ASeniorProjectCharacter* Player) = 0;
	virtual void Use(ASeniorProjectCharacter* Player) {}
};
