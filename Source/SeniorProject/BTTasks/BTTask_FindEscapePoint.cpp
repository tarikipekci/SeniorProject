#include "BTTask_FindEscapePoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "SeniorProject/Characters/Animal.h"
#include "SeniorProject/Components/RLifeComponent.h"
#include "SeniorProject/Controllers/Animal_AIController.h"

UBTTask_FindEscapePoint::UBTTask_FindEscapePoint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Find Escape Point";
	EscapeDistance = 500.f;
	ExtentValue = 100.f;
}

EBTNodeResult::Type UBTTask_FindEscapePoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Only execute on the server
	if(OwnerComp.GetNetMode() == NM_Client)
	{
		return EBTNodeResult::Failed;
	}

	auto* const AnimalController = Cast<AAnimal_AIController>(OwnerComp.GetAIOwner());
	auto* const Animal = Cast<AAnimal>(AnimalController->GetPawn());
	if(FMath::Abs(
		Animal->GetActorLocation().Size() - OwnerComp.GetBlackboardComponent()->GetValueAsVector("PatrolPathPoint").
		                                              Size()) >= EscapeDistance)
	{
		if(Animal->GetLifeComponent()->GetCurrentHitPoints() != Animal->GetLifeComponent()->GetMaxHitPoints())
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", false);
		}
	}

	FVector EscapeLocation = Animal->GetActorLocation() + Animal->GetActorForwardVector() * EscapeDistance;

	auto* const NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if(NavSystem)
	{
		FNavLocation NavLocation;
		if(NavSystem->ProjectPointToNavigation(EscapeLocation, NavLocation, FVector(ExtentValue)))
		{
			EscapeLocation = NavLocation.Location;
		}
		else
		{
			return EBTNodeResult::Failed;
		}
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), EscapeLocation);
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
