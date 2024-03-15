// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "HudWidget.generated.h"

class UWidgetSwitcher;
class UButton;
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

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateInventoryBuilding();

public:
	UPROPERTY(BlueprintReadWrite ,meta=(BindWidget))
	UUserWidget* WBP_PlayerInventory;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UUserWidget* WBP_InteractableInventory;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UUserWidget* WBP_Interaction;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UUserWidget* WBP_CraftingMenu;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* InventoryButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UWidgetSwitcher* WS_TabPanel;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* CraftingButton;
};
