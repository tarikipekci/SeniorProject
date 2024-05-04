// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Animal_AIController.generated.h"

class ASeniorProjectCharacter;
struct FAIStimulus;
/**
 * 
 */
UCLASS()
class SENIORPROJECT_API AAnimal_AIController : public AAIController
{
	GENERATED_BODY()

public:
	explicit AAnimal_AIController(FObjectInitializer const& ObjectInitializer);

	ASeniorProjectCharacter* GetDetectedPlayer() const {return DetectedPlayer;}

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY()
	ASeniorProjectCharacter* DetectedPlayer;

	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);
};
