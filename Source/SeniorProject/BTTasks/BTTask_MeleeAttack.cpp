// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MeleeAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SeniorProject/Characters/Animal.h"

UBTTask_MeleeAttack::UBTTask_MeleeAttack()
{
	NodeName = TEXT("Meele Attack");
}

EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const OutOfRange = !OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
	if(OutOfRange)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	auto const* const AnimalController = OwnerComp.GetAIOwner();
	auto* const Animal = Cast<AAnimal>(AnimalController->GetPawn());

	if(auto* const CombatInterface = Cast<ICombatInterface>(Animal))
	{
		if(MontageHasFinished(Animal))
		{
			CombatInterface->Execute_MeleeAttack(Animal);
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
