#include "BTService_IsPlayerInMeleeRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SeniorProject/Characters/Animal.h"
#include "SeniorProject/Characters/SeniorProjectCharacter.h"
#include "SeniorProject/Controllers/Animal_AIController.h"

UBTService_IsPlayerInMeleeRange::UBTService_IsPlayerInMeleeRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Melee Range");
}

void UBTService_IsPlayerInMeleeRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	if (OwnerComp.GetNetMode() == NM_Client)
	{
		return;
	}

	auto const* const AnimalController = Cast<AAnimal_AIController>(OwnerComp.GetAIOwner());
	if (AnimalController == nullptr)
	{
		return;
	}

	auto const* const Animal = Cast<AAnimal>(AnimalController->GetPawn());
	if (Animal == nullptr)
	{
		return;
	}

	auto const* const Player = AnimalController->GetDetectedPlayer();
	if (Player == nullptr)
	{
		return;
	}

	auto* const BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		return;
	}

	float const DistanceToPlayer = Animal->GetDistanceTo(Player);
	bool const bIsPlayerInMeleeRange = DistanceToPlayer <= MeleeRange;

	BlackboardComp->SetValueAsBool(GetSelectedBlackboardKey(), bIsPlayerInMeleeRange);
}
