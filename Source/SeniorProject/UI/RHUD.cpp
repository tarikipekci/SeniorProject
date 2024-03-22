// Fill out your copyright notice in the Description page of Project Settings.


#include "RHUD.h"

#include "HudWidget.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/WidgetSwitcher.h"
#include "SeniorProject/Characters/SeniorProjectCharacter.h"

void ARHUD::CreateHudWidgets()
{
	if(HudWidgetClass.Get() != nullptr)
	{
		HudWidget = CreateWidget<UHudWidget>(GetWorld(), HudWidgetClass.Get());
		HudWidget->SetHudWidgets(this);
		HudWidget->AddToViewport();
	}
}

void ARHUD::ToggleVisibilityOfInventory(APlayerController* PlayerController)
{
	if(PlayerController)
	{
		if(InteractableInventory && PlayerInventoryWidget && PlayerInventoryWidget->IsVisible())
		{
			if(InteractableInventory->IsVisible())
			{
				ASeniorProjectCharacter* Player = Cast<ASeniorProjectCharacter>(PlayerController->GetPawn());
				Player->Client_CloseInventory();
			}
			FInputModeGameOnly InputModeGameOnly;
			PlayerController->SetInputMode(InputModeGameOnly);
			PlayerController->SetShowMouseCursor(false);
			PlayerInventoryWidget->SetVisibility(ESlateVisibility::Hidden);
			InventoryButton->SetVisibility(ESlateVisibility::Hidden);
			CraftingButton->SetVisibility(ESlateVisibility::Hidden);
			CraftingMenu->SetVisibility(ESlateVisibility::Hidden);
			InteractionWidget->SetRenderOpacity(1);
			PlayerController->SetIgnoreMoveInput(false);
			PlayerController->SetIgnoreLookInput(false);
		}
		else
		{
			FInputModeGameAndUI InputModeGameAndUIOnly;
			PlayerController->SetInputMode(InputModeGameAndUIOnly);
			PlayerController->SetIgnoreMoveInput(true);
			PlayerController->SetIgnoreLookInput(true);
			PlayerController->SetShowMouseCursor(true);
			PlayerInventoryWidget->SetVisibility(ESlateVisibility::Visible);
			InventoryButton->SetVisibility(ESlateVisibility::Visible);
			CraftingButton->SetVisibility(ESlateVisibility::Visible);
			CraftingMenu->SetVisibility(ESlateVisibility::Visible);
			TabPanel->SetActiveWidgetIndex(0);
		}
	}
}

void ARHUD::BeginPlay()
{
	Super::BeginPlay();

	CreateHudWidgets();
}