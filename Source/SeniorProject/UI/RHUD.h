// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "GameFramework/HUD.h"
#include "RHUD.generated.h"

class UWidgetSwitcher;
class UButton;
/**
 * 
 */
UCLASS()
class SENIORPROJECT_API ARHUD : public AHUD
{
	GENERATED_BODY()
public:
	void CreateHudWidgets();

	UFUNCTION(BlueprintCallable)
	void ToggleVisibilityOfInventory(APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable)
	void ToggleEscMenu(APlayerController* PlayerController);

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

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* CraftingMenu;

	UPROPERTY(BlueprintReadWrite)
	UWidgetSwitcher* TabPanel;
	
	UPROPERTY(BlueprintReadWrite)
	UButton* InventoryButton;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* EscMenu;

	UPROPERTY(BlueprintReadWrite)
	UButton* CraftingButton;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HudWidgetClass;
};
