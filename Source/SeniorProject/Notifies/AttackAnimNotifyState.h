// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AttackAnimNotifyState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAINotifyBegin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAINotifyTick);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAINotifyEnd);

UCLASS()
class SENIORPROJECT_API UAttackAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
	                        const FAnimNotifyEventReference& EventReference) override;

	UFUNCTION()
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	                         const FAnimNotifyEventReference& EventReference) override;

	UFUNCTION()
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                       const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(BlueprintAssignable)
	FOnAINotifyBegin OnNotifyBegin;

	UPROPERTY(BlueprintAssignable)
	FOnAINotifyTick OnNotifyTick;

	UPROPERTY(BlueprintAssignable)
	FOnAINotifyEnd OnNotifyEnd;
};
