// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_IncrementPathIndex.generated.h"

/**
 * 
 */
UCLASS()
class SENIORPROJECT_API UBTTask_IncrementPathIndex : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTTask_IncrementPathIndex(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	enum class EDirectionType {Forward, Reverse};

	EDirectionType Direction = EDirectionType::Forward;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI",meta=(AllowPrivateAccess = "true"))
	bool bBiDirectional = false;
};
