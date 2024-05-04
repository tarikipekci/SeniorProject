// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animal.generated.h"

class APatrolPath;
class URLifeComponent;
class UBehaviorTree;

UCLASS()
class SENIORPROJECT_API AAnimal : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAnimal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess = "true"))
	UBehaviorTree* Tree;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UBehaviorTree* GetBehaviourTree() const {return Tree;}

	UFUNCTION(BlueprintCallable)
	float GetWalkSpeed() const {return WalkSpeed;}

	UFUNCTION(BlueprintCallable)
	float GetGallopSpeed() const {return GallopSpeed;}

	UFUNCTION(BlueprintCallable)
	URLifeComponent* GetLifeComponent() const {return LifeComp;}

	UFUNCTION(BlueprintCallable)
	APatrolPath* GetPatrolPath() const {return PatrolPath;}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleCollision;

	UFUNCTION()
	void ChangeMovementSpeed(float NewSpeed);

private:
	UPROPERTY()
	float WalkSpeed;

	UPROPERTY()
	float GallopSpeed;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI",meta=(AllowPrivateAccess="true"))
	APatrolPath* PatrolPath;

protected:
	UPROPERTY(VisibleAnywhere)
	URLifeComponent* LifeComp;
};
