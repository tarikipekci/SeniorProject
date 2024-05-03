// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPlayerLocation.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "SeniorProject/Characters/Animal.h"
#include "SeniorProject/Controllers/Animal_AIController.h"

UBTTask_FindPlayerLocation::UBTTask_FindPlayerLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Get Player Character
	if(auto* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		//Get player location to use as an origin
		auto const PlayerLocation = Player->GetActorLocation();
		if(SearchRandom)
		{
			FNavLocation Location;

			//Get the navigation system and generate a random location near the player
			if(auto* const NavSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				//Try to get a random location near the player
				if(NavSystem->GetRandomPointInNavigableRadius(PlayerLocation, SearchRadius, Location))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Location.Location);
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
			}
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerLocation);
			auto* const AnimalController = Cast<AAnimal_AIController>(OwnerComp.GetAIOwner());
			auto* const Animal = Cast<AAnimal>(AnimalController->GetPawn());
			Animal->ChangeMovementSpeed(Animal->GetGallopSpeed());
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
