#include "BTTask_Escape.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "SeniorProject/Controllers/Animal_AIController.h"

class UNavigationSystemV1;

UBTTask_Escape::UBTTask_Escape()
{
	NodeName = "Escape";
}

EBTNodeResult::Type UBTTask_Escape::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Only execute on the server
	if(OwnerComp.GetNetMode() == NM_Client)
	{
		return EBTNodeResult::Failed;
	}

	// Get target location from blackboard via the animal's controller
	if(auto* const AnimalController = Cast<AAnimal_AIController>(OwnerComp.GetAIOwner()))
	{
		auto const EscapeLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

		// Move to the escape location
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(AnimalController, EscapeLocation);

		// Finish with success
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
