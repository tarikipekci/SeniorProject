#include "BTService_ChangeSpeed.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SeniorProject/Characters/Animal.h"

UBTService_ChangeSpeed::UBTService_ChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Speed");
}

void UBTService_ChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	if (OwnerComp.GetNetMode() == NM_Client)
	{
		return;
	}

	if (auto const AnimalController = OwnerComp.GetAIOwner())
	{
		if (auto* const Animal = Cast<AAnimal>(AnimalController->GetPawn()))
		{
			auto* const BlackboardComp = OwnerComp.GetBlackboardComponent();
			if (BlackboardComp->GetValueAsBool(CanSeePlayerKey.SelectedKeyName))
			{
				CurrentSpeed = Animal->GetGallopSpeed();
				Animal->Server_ChangeMovementSpeed(CurrentSpeed);
			}
			else
			{
				CurrentSpeed = Animal->GetWalkSpeed();
				Animal->Server_ChangeMovementSpeed(CurrentSpeed);
			}
		}
	}
}
