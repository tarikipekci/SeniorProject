// Fill out your copyright notice in the Description page of Project Settings.


#include "HudWidget.h"

#include "RHUD.h"

void UHudWidget::SetHudWidgets(ARHUD* Hud)
{
	if(Hud == nullptr)
	{
		
		return;
	}
	Hud->PlayerInventoryWidget = WBP_PlayerInventory;
	Hud->InteractableInventory = WBP_InteractableInventory;
	Hud->InteractionWidget = WBP_Interaction;
}
