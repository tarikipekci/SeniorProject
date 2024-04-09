// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RDamageComponent.generated.h"


enum class EDamageType : uint8;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SENIORPROJECT_API URDamageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URDamageComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitAnimations();

	UFUNCTION()
	int GetDamageAmount() { return DamageAmount;}

	UFUNCTION()
	UAnimMontage* GetAttackAnimMontage() {return PlayerAttackMontage;}
	
	UFUNCTION()
	void ToggleCollisionOfTool();
	
	UFUNCTION()
	void CheckOverlapping();

	UFUNCTION()
	void OnAnimationFinish();

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EDamageType DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int DamageAmount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* PlayerAttackMontage;

	bool bIsAnimPlayed;
};


