// Fill out your copyright notice in the Description page of Project Settings.


#include "RLifeComponent.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "SeniorProject/Characters/Animal.h"
#include "SeniorProject/Controllers/Animal_AIController.h"

// Sets default values for this component's properties
URLifeComponent::URLifeComponent()
{
}

// Called when the game starts
void URLifeComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHitPoints = MaxHitPoints;
}

void URLifeComponent::DecreaseHitPoints(int DecreasedAmount)
{
	if(CurrentHitPoints - DecreasedAmount <= 0)
	{
		GetOwner()->Destroy();
		DropLootItems();
	}
	else
	{
		CurrentHitPoints -= DecreasedAmount;
		Server_WarnEnemyAI();
	}
}

void URLifeComponent::Server_WarnEnemyAI_Implementation()
{
	AAnimal* Animal = Cast<AAnimal>(GetOwner());
	if(Animal)
	{
		AAnimal_AIController* AnimalController = Cast<AAnimal_AIController>(Animal->GetController());
		if(AnimalController)
		{
			AnimalController->GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", true);
			AnimalController->SetDetectedPlayer(DamagingPlayer);
		}
	}
}

void URLifeComponent::DropLootItems()
{
	if(GetOwner()->HasAuthority())
	{
		for(auto ItemClass : LootItemClasses)
		{
			int RandomNumberX = FMath::RandRange(0, 200);
			int RandomNumberY = FMath::RandRange(0, 200);
			FVector RandomVector = FVector(RandomNumberX, RandomNumberY, 500);
			FVector SpawnVector = GetOwner()->GetActorLocation() + RandomVector;

			FVector Start = SpawnVector;
			FVector End = SpawnVector - FVector(0, 0, 1000.0f);

			FHitResult HitResult;
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(GetOwner());

			if(GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
			{
				SpawnVector = HitResult.Location;
			}
			AItem* DroppedLoot = GetWorld()->SpawnActor<AItem>(ItemClass, SpawnVector, FRotator(0, 0, 0));
		}
	}
}
