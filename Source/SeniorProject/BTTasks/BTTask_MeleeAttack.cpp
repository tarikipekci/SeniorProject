#include "BTTask_MeleeAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SeniorProject/Characters/Animal.h"
#include "SeniorProject/Characters/SeniorProjectCharacter.h"
#include "SeniorProject/Controllers/Animal_AIController.h"

UBTTask_MeleeAttack::UBTTask_MeleeAttack()
{
	NodeName = TEXT("Melee Attack");
}

EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Only execute on the server
	if(OwnerComp.GetNetMode() == NM_Client)
	{
		return EBTNodeResult::Failed;
	}

	auto const OutOfRange = !OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
	if(OutOfRange)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	auto const* const AnimalController = Cast<AAnimal_AIController>(OwnerComp.GetAIOwner());
	auto* const Animal = Cast<AAnimal>(AnimalController->GetPawn());

	if(MontageHasFinished(Animal))
	{
		if(AnimalController->GetDetectedPlayer() && !AnimalController->GetDetectedPlayer()->GetIsDead())
		{
			Animal->Server_MeleeAttack();
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", false);
		}
	}
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

bool UBTTask_MeleeAttack::MontageHasFinished(AAnimal* const Animal)
{
	if(Animal->GetVelocity().Size() > 0)
	{
		return Animal->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(Animal->GetUpperBodyMontage());
	}
	return Animal->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(Animal->GetNormalMontage());
}
