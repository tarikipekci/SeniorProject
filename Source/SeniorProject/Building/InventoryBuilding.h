// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableBuilding.h"
#include "InventoryBuilding.generated.h"

class UHudWidget;
/**
 * 
 */
UCLASS()
class SENIORPROJECT_API AInventoryBuilding : public AInteractableBuilding
{
	GENERATED_BODY()
	AInventoryBuilding();

public:
	virtual void Interact(ASeniorProjectCharacter* Player) override;

protected:
	UPROPERTY(BlueprintReadWrite, Replicated)
	bool bOpened;

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	URInventoryComponent* InventoryComp;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComp;

	UFUNCTION(Server,Reliable)
	void Server_OpenInventory(ASeniorProjectCharacter* Player);

	UFUNCTION(Server,Reliable)
	void Server_CloseInventory();

	UFUNCTION(BlueprintCallable)
	void SetIsOpened(bool bIsOpened) { bOpened = bIsOpened;}
};
