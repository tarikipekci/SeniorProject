// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AttackAnimNotifyState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNotifyBegin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNotifyTick);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNotifyEnd);

UCLASS()
class SENIORPROJECT_API UAttackAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;

	UFUNCTION()
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

	UFUNCTION()
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(BlueprintAssignable)
	FOnNotifyBegin OnNotifyBegin;

	UPROPERTY(BlueprintAssignable)
	FOnNotifyTick OnNotifyTick;

	UPROPERTY(BlueprintAssignable)
	FOnNotifyEnd OnNotifyEnd;
};
