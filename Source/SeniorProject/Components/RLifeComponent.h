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

	UFUNCTION(BlueprintCallable)
	void SetDamagingPlayer(ASeniorProjectCharacter* Player) {DamagingPlayer = Player;}

	UFUNCTION(BlueprintCallable)
	int GetCurrentHitPoints() const {return CurrentHitPoints;}

	UFUNCTION(BlueprintCallable)
	int GetMaxHitPoints() const {return MaxHitPoints;}

	UFUNCTION(Server,Reliable)
	void Server_WarnEnemyAI();
	
	UFUNCTION()
	void DropLootItems();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentHitPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxHitPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDamageType EffectiveDamageType;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<TSubclassOf<AItem>> LootItemClasses;

	UPROPERTY()
	ASeniorProjectCharacter* DamagingPlayer;
};
