// Fill out your copyright notice in the Description page of Project Settings.


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
	auto const* const AnimalController = Cast<AAnimal_AIController>(OwnerComp.GetAIOwner());
	auto const* const Animal = Cast<AAnimal>(AnimalController->GetPawn());
	auto const* const Player = AnimalController->GetDetectedPlayer();

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(),
	                                                   Animal->GetDistanceTo(Player) <= MeleeRange);
}
