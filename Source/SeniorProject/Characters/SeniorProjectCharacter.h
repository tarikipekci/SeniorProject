// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "SeniorProjectCharacter.generated.h"


class APickups;

UCLASS(config=Game)
class ASeniorProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

public:
	ASeniorProjectCharacter();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class URPlayerStatComponent* PlayerStatComp;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class URLineTraceComponent* LineTraceComp;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class URInteractComponent* InteractComp;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class URInventoryComponent* InventoryComp;

protected:
	float InteractRange;
	float RespawnDuration;

	//Stamina Properties
	bool bIsSprinting;
	float StaminaDecrementTimerDuration;
	float JumpStaminaCost;

	//Timers
	FTimerHandle SprintingHandle;
	FTimerHandle DestroyHandle;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void StartSprinting();

	UFUNCTION(BlueprintCallable)
	void StopSprinting();

	void HandleSprinting();

	void AttemptJump();

	void Die();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void MultiDie();
	bool MultiDie_Validate();
	void MultiDie_Implementation();

	UFUNCTION(BlueprintCallable)
	void Attack();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAttack(AActor* Actor);
	bool ServerAttack_Validate(AActor* Actor);
	void ServerAttack_Implementation(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	void Interact();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerInteract(AActor* Actor);
	bool ServerInteract_Validate(AActor* Actor);
	void ServerInteract_Implementation(AActor* Actor);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();

	// Calls Destroy()
	void CallDestroy();

public:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
