// Fill out your copyright notice in the Description page of Project Settings.


#include "LootableActor.h"

#include "SeniorProject/Components/RLifeComponent.h"

// Sets default values
ALootableActor::ALootableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	LifeComp = CreateDefaultSubobject<URLifeComponent>("LifeComponent");
}

// Called when the game starts or when spawned
void ALootableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALootableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

