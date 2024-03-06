// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HudWidget.generated.h"

class ARHUD;
/**
 * 
 */
UCLASS()
class SENIORPROJECT_API UHudWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void SetHudWidgets(ARHUD* Hud);

public:
	UPROPERTY(BlueprintReadWrite ,meta=(BindWidget))
	UUserWidget* WBP_PlayerInventory;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UUserWidget* WBP_InteractableInventory;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UUserWidget* WBP_Interaction;
};
