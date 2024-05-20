#include "BTTask_FindPlayerLocation.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SeniorProject/Characters/Animal.h"
#include "SeniorProject/Characters/SeniorProjectCharacter.h"
#include "SeniorProject/Controllers/Animal_AIController.h"

UBTTask_FindPlayerLocation::UBTTask_FindPlayerLocation(FObjectInitializer const& ObjectInitializer) :
    UBTTask_BlackboardBase(ObjectInitializer)
{
    NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Only execute on the server
    if (OwnerComp.GetNetMode() == NM_Client)
    {
        return EBTNodeResult::Failed;
    }

    auto* const AnimalController = Cast<AAnimal_AIController>(OwnerComp.GetAIOwner());
    auto* const Animal = Cast<AAnimal>(AnimalController->GetPawn());

    // Get Player Character
    if (auto* const Player = AnimalController->GetDetectedPlayer())
    {
        // Get player location to use as an origin
        auto const PlayerLocation = Player->GetActorLocation();
        if (SearchRandom)
        {
            FNavLocation Location;

            // Get the navigation system and generate a random location near the player
            if (auto* const NavSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
            {
                // Try to get a random location near the player
                if (NavSystem->GetRandomPointInNavigableRadius(PlayerLocation, SearchRadius, Location))
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
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
            return EBTNodeResult::Succeeded;
        }
    }
    return EBTNodeResult::Failed;
}
