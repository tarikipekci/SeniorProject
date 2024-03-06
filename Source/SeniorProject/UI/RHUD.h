// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RHUD.generated.h"

/**
 * 
 */
UCLASS()
class SENIORPROJECT_API ARHUD : public AHUD
{
	GENERATED_BODY()
public:
	void CreateHudWidgets();
	void SetHudWidgets(ARHUD* Hud);

	UFUNCTION(BlueprintCallable)
	void ToggleVisibilityOfInventory(APlayerController* PlayerController);
	
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite)
	class UHudWidget* HudWidget;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* PlayerInventoryWidget;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* InteractableInventory;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* InteractionWidget;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HudWidgetClass;
};
