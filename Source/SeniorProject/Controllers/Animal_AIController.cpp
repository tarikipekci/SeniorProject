// Fill out your copyright notice in the Description page of Project Settings.


#include "Animal_AIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Sight.h"
#include "SeniorProject/Characters/Animal.h"
#include "SeniorProject/Characters/SeniorProjectCharacter.h"

AAnimal_AIController::AAnimal_AIController(FObjectInitializer const& ObjectInitializer)
{
	SetupPerceptionSystem();
}

void AAnimal_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if(AAnimal* const  Animal = Cast<AAnimal>(InPawn))
	{
		if(UBehaviorTree* const Tree = Animal->GetBehaviourTree())
		{
			UBlackboardComponent* BlackboardComp;
			UseBlackboard(Tree->BlackboardAsset, BlackboardComp);
			Blackboard = BlackboardComp;
			RunBehaviorTree(Tree);
		}
	}
}

void AAnimal_AIController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if(SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		SightConfig->SightRadius = 500.0f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;
		SightConfig->PeripheralVisionAngleDegrees = 90.f;
		SightConfig->SetMaxAge(5.f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this,&AAnimal_AIController::OnTargetDetected);
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}
}

void AAnimal_AIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if(auto* const Player = Cast<ASeniorProjectCharacter>(Actor))
	{
		GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
		DetectedPlayer = Player;
	}
}
