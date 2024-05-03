// Fill out your copyright notice in the Description page of Project Settings.


#include "LootableActor.h"

#include "Item.h"
#include "SeniorProject/Components/RLifeComponent.h"

// Sets default values
ALootableActor::ALootableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	LifeComp = CreateDefaultSubobject<URLifeComponent>("LifeComponent");
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = MeshComp;
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

void ALootableActor::DropLootItems()
{
	if(HasAuthority())
	{
		for(auto ItemClass : LootItemClasses)
		{
			int RandomNumber = FMath::RandRange(0, 200);
			FVector RandomVector = FVector(RandomNumber, RandomNumber, 0);
			AItem* DroppedLoot = GetWorld()->SpawnActor<AItem>(ItemClass, GetActorLocation() + RandomVector,
			                                                   FRotator(0, 0, 0));
		}
	}
}


