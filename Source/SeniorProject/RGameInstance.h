// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RGameInstance.generated.h"

UCLASS()
class SENIORPROJECT_API URGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	FString GetPlayerName() const { return PlayerName; }

	UFUNCTION(BlueprintCallable)
	void SetPlayerName(FString Name) { PlayerName = Name; }

	virtual void Init() override;
	
private:
	FString PlayerName;
};
