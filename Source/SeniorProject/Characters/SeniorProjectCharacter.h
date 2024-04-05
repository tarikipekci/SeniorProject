// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/WidgetComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "SeniorProject/Structs/FItemData.h"
#include "SeniorProjectCharacter.generated.h"


class URCraftComponent;
class AInventoryBuilding;
class URInventoryComponent;
class URInteractComponent;
class URLineTraceComponent;
class URPlayerStatComponent;
class UWidgetComponent;

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
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

public:
	ASeniorProjectCharacter();
	
	//Player Components
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	URPlayerStatComponent* PlayerStatComp;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	URLineTraceComponent* LineTraceComp;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	URInteractComponent* InteractComp;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	URInventoryComponent* InventoryComp;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	URCraftComponent* CraftComp;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UWidgetComponent* PlayerNameTagComp;

protected:
	float RespawnDuration;

	UPROPERTY(BlueprintReadWrite)
	bool bCanFillWater;

	//Stamina Properties
	bool bIsSprinting;
	float StaminaDecrementTimerDuration;
	float JumpStaminaCost;

	float DefaultWalkSpeed;
	float SprintSpeed;

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

	UFUNCTION(BlueprintCallable)
	void HandleSprinting();

	UFUNCTION(BlueprintCallable)
	void AttemptJump();

	UFUNCTION(BlueprintCallable)
	void Die();

	UFUNCTION(Server, Reliable)
	void Server_UsePickup(AItem* Item);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Die();

	UFUNCTION(BlueprintCallable)
	void Attack();

	UFUNCTION(Server, Reliable)
	void Server_Attack(AActor* Actor);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();

	// Calls Destroy()
	void CallDestroy();

	// Gets player's inventory component
	UFUNCTION(BlueprintCallable)
	URInventoryComponent* GetInventoryComp() const { return InventoryComp; }

public:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	void UsePickup(AItem* Item);

	UFUNCTION(Client, Reliable)
	void Client_OpenInventory();

	UFUNCTION(Client, Reliable)
	void Client_CloseInventory();

	UFUNCTION(Server, Reliable)
	void Server_CloseInventoryBuilding();

	UFUNCTION(Client,Reliable)
	void Client_CloseInteractionWidget();

	UFUNCTION(Server, Reliable)
	void Server_SetItemOwnership(AInventoryBuilding* SpawnedItem);

	UFUNCTION(BlueprintCallable)
	void EquipItem(AItem* EquippedItem);
	
	UFUNCTION(NetMulticast, Reliable)
	void Server_EquipItem(AItem* EquippedItem);

	UFUNCTION(BlueprintCallable)
	void UnEquipItem(AItem* UnequippedItem);

	UFUNCTION(NetMulticast, Reliable)
	void Server_UnEquipItem(AItem* UnequippedItem);

	UFUNCTION()
	bool GetCanFillWater(){return bCanFillWater;}

	UFUNCTION(BlueprintImplementableEvent)
	bool IsPlayerHoldingBottle();

	UFUNCTION(BlueprintImplementableEvent)
	void GiveEmptyBottleBack(AItem* FullBottle);
	
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/** Returns InteractComponent subobject **/
	FORCEINLINE class URInteractComponent* GetInteractComponent() const { return InteractComp; }
};
