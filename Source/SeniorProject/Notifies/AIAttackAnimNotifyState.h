// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AIAttackAnimNotifyState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAIAttackBegin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAIOnAttack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAIAttackEnd);

UCLASS()
class SENIORPROJECT_API UAIAttackAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	                         const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
	                        const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                       const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(BlueprintAssignable)
	FAIAttackBegin AIAttackBegin;

	UPROPERTY(BlueprintAssignable)
	FAIOnAttack AIOnAttack;

	UPROPERTY(BlueprintAssignable)
	FAIAttackEnd AIAttackEnd;
};
