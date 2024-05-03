// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindRandomLocation.h"

#include "Animal.h"
#include "Animal_AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

class AAnimal;

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer)
{
	SearchRadius = 1500.f;
	NodeName = "Find Random Location In NavMesh";
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Get AI controller and its animal
	if(auto* const AnimalController = Cast<AAnimal_AIController>(OwnerComp.GetAIOwner()))
	{
		if(AAnimal* const Animal = Cast<AAnimal>(AnimalController->GetPawn()))
		{
            // Obtain animal location to use as an origin
            auto const Origin = Animal->GetActorLocation();
            Animal->ChangeMovementSpeed(Animal->GetWalkSpeed());
			
			//Get the navigation system and generate a random location
			if(auto* const NavSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				FNavLocation Location;
				if(NavSystem->GetRandomPointInNavigableRadius(Origin, SearchRadius, Location))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Location.Location);
				}
				//Finish with success
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}