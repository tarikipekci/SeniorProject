// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatInterface.h"
#include "GameFramework/Character.h"
#include "Animal.generated.h"

class APatrolPath;
class URLifeComponent;
class UBehaviorTree;

UCLASS()
class SENIORPROJECT_API AAnimal : public ACharacter, public ICombatInterface
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

	UAnimMontage* GetUpperBodyMontage() const {return UpperBodyMontage;}

	UAnimMontage* GetNormalMontage() const {return NormalMontage;}

	virtual int MeleeAttack_Implementation() override;
	
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_PlayAttackAnimation();

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation", meta=(AllowPrivateAccess="true"))
	UAnimMontage* UpperBodyMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation", meta=(AllowPrivateAccess="true"))
	UAnimMontage* NormalMontage;

protected:
	UPROPERTY(VisibleAnywhere)
	URLifeComponent* LifeComp;
};
