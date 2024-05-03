// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SeniorProject/Environment/Item.h"
#include "RLifeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SENIORPROJECT_API URLifeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URLifeComponent();

	UFUNCTION()
	void DecreaseHitPoints(int DecreasedAmount);

	UFUNCTION(BlueprintCallable)
	EDamageType GetEffectiveDamageType() {return EffectiveDamageType;}

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentHitPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxHitPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDamageType EffectiveDamageType;
};
