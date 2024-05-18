// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LootableActor.generated.h"

class AItem;
enum class EDamageType : uint8;
class URLifeComponent;

UCLASS()
class SENIORPROJECT_API ALootableActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALootableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	URLifeComponent* GetLifeComponent() {return LifeComp;}
	

protected:
	UPROPERTY(VisibleAnywhere)
	URLifeComponent* LifeComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MeshComp;
};
