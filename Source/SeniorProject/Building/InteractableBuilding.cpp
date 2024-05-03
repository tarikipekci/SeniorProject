// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableBuilding.h"

#include "Components/CapsuleComponent.h"

AInteractableBuilding::AInteractableBuilding()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = MeshComp;
	InteractCollision = CreateDefaultSubobject<UCapsuleComponent>("InteractCollision");
	InteractCollision->SetupAttachment(MeshComp);
	InteractCollision->SetRelativeLocation(FVector::ZeroVector);
}

void AInteractableBuilding::Interact(ASeniorProjectCharacter* Player)
{
}

void AInteractableBuilding::Use(ASeniorProjectCharacter* Player)
{
	IInteractableInterface::Use(Player);
}
