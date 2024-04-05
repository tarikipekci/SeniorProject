// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableInterface.h"
#include "GameFramework/Actor.h"
#include "InteractableNature.generated.h"

UCLASS()
class SENIORPROJECT_API AInteractableNature : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractableNature();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void Interact(ASeniorProjectCharacter* Player) override;

	UFUNCTION(BlueprintCallable)
	virtual void Use(ASeniorProjectCharacter* Player) override;

	UFUNCTION(BlueprintCallable)
	bool GetIsInteractable() { return bIsInteractable; }

	UFUNCTION(BlueprintCallable)
	void SetIsInteractable(bool IsInteractable) {bIsInteractable = IsInteractable;}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> RewardItemClass;

	UPROPERTY()
	bool bIsInteractable;
};
