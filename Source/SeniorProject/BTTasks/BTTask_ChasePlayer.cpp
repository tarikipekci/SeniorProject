#include "BTTask_ChasePlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "SeniorProject/Controllers/Animal_AIController.h"

UBTTask_ChasePlayer::UBTTask_ChasePlayer(FObjectInitializer const& ObjectInitializer) : UBTTask_BlackboardBase(ObjectInitializer)
{
    NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Only execute on the server
    if (OwnerComp.GetNetMode() == NM_Client)
    {
        return EBTNodeResult::Failed;
    }

    // Get target location from blackboard via the animal's controller
    if (auto* const AnimalController = Cast<AAnimal_AIController>(OwnerComp.GetAIOwner()))
    {
        auto const PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

        // Move to the player's location
        UAIBlueprintHelperLibrary::SimpleMoveToLocation(AnimalController, PlayerLocation);

        // Finish with success
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return EBTNodeResult::Succeeded;
    }
    return EBTNodeResult::Failed;
}
