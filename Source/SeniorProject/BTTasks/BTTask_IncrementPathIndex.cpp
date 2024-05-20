#include "BTTask_IncrementPathIndex.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SeniorProject/Characters/Animal.h"
#include "SeniorProject/Controllers/Animal_AIController.h"
#include "SeniorProject/Environment/PatrolPath.h"

UBTTask_IncrementPathIndex::UBTTask_IncrementPathIndex(FObjectInitializer const& ObjectInitializer) :
    UBTTask_BlackboardBase(ObjectInitializer)
{
    NodeName = TEXT("Increment Path Index");
}

EBTNodeResult::Type UBTTask_IncrementPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Only execute on the server
    if(OwnerComp.GetNetMode() == NM_Client)
    {
        return EBTNodeResult::Failed;
    }

    // Try and get the AI controller
    if(auto* const AnimalController = Cast<AAnimal_AIController>(OwnerComp.GetAIOwner()))
    {
        // Try and get the Animal
        if(auto* const Animal = Cast<AAnimal>(AnimalController->GetPawn()))
        {
            // Try and get the blackboard component
            if(auto* const BlackboardComp = OwnerComp.GetBlackboardComponent())
            {
                // Get number of patrol points, min and max indices
                auto const NumberOfPoints = Animal->GetPatrolPath()->Num();
                auto const MinIndex = 0;
                auto const MaxIndex = NumberOfPoints - 1;
                auto Index = BlackboardComp->GetValueAsInt(GetSelectedBlackboardKey());

                // Change direction if we are at the first or last index
                if(bBiDirectional)
                {
                    if(Index >= MaxIndex && Direction == EDirectionType::Forward)
                    {
                        Direction = EDirectionType::Reverse;
                    }
                    else if(Index <= MinIndex && Direction == EDirectionType::Reverse)
                    {
                        Direction = EDirectionType::Forward;
                    }
                }

                // Set new value of path index
                BlackboardComp->SetValueAsInt(GetSelectedBlackboardKey(),
                                              (Direction == EDirectionType::Forward ? ++Index : --Index) %
                                              NumberOfPoints);

                // Finish with success
                FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
                return EBTNodeResult::Succeeded;
            }
        }
    }
    // Something went wrong so fail
    return EBTNodeResult::Failed;
}
