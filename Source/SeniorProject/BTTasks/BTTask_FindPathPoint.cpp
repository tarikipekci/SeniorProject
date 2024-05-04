// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPathPoint.h"


#include "BehaviorTree/BlackboardComponent.h"
#include "SeniorProject/Characters//Animal.h"
#include "SeniorProject/Controllers/Animal_AIController.h"
#include "SeniorProject/Environment/PatrolPath.h"

class AAnimal;

UBTTask_FindPathPoint::UBTTask_FindPathPoint(FObjectInitializer const& ObjectInitializer) : UBTTask_BlackboardBase(
	ObjectInitializer)
{
	NodeName = TEXT("Find Path Point");
}

EBTNodeResult::Type UBTTask_FindPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Attempt to get the Animal's controller
	if(auto* const AnimalController = Cast<AAnimal_AIController>(OwnerComp.GetAIOwner()))
	{
		//Attempt to get the blackboard component from the behaviour tree
		if(auto* const BlackboardComp = OwnerComp.GetBlackboardComponent())
		{
			//Get the current patrol path index from the blackboard
			auto const Index = BlackboardComp->GetValueAsInt(GetSelectedBlackboardKey());

			//Get the Animal actor

			if(AAnimal* const Animal = Cast<AAnimal>(AnimalController->GetPawn()))
			{
				//Get the current patrol path vector from the Animal - this is local to the patrol path actor
				auto const Point = Animal->GetPatrolPath()->GetPatrolPoint(Index);

				//Convert the local vector to a global point
				auto const GlobalPoint = Animal->GetPatrolPath()->GetActorTransform().TransformPosition(Point);
				BlackboardComp->SetValueAsVector(PatrolPathVectorKey.SelectedKeyName, GlobalPoint);

				//Finish with success
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
