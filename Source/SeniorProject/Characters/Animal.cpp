// Fill out your copyright notice in the Description page of Project Settings.


#include "Animal.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SeniorProject/Components/RLifeComponent.h"
#include "SeniorProject/Components/RPlayerStatComponent.h"
#include "SeniorProject/Notifies/AIAttackAnimNotifyState.h"

// Sets default values
AAnimal::AAnimal()
{
	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>("CapsuleCollision");
	CapsuleCollision->SetRelativeLocation(FVector::ZeroVector);
	CapsuleCollision->SetupAttachment(RootComponent);
	LifeComp = CreateDefaultSubobject<URLifeComponent>("LifeComponent");
	WalkSpeed = 200;
	GallopSpeed = 400;
	MouthCollisionBoxExtentSize = 10.f;
	DamageAmount = 30.f;
	MouthCollision = CreateDefaultSubobject<UBoxComponent>("MouthCollision");
}

// Called when the game starts or when spawned
void AAnimal::BeginPlay()
{
	Super::BeginPlay();
	Server_ChangeMovementSpeed(WalkSpeed);
	if(MouthCollision)
	{
		MouthCollision->SetBoxExtent(FVector(MouthCollisionBoxExtentSize, MouthCollisionBoxExtentSize,
		                                     MouthCollisionBoxExtentSize), false);
		FAttachmentTransformRules const Rules
		{
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			false
		};
		MouthCollision->AttachToComponent(GetMesh(), Rules, "Mouth");
	}
	Server_InitAnimations();
}

// Called to bind functionality to input
void AAnimal::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if(GetNetMode() == NM_ListenServer)
	{
		Super::SetupPlayerInputComponent(PlayerInputComponent);
	}
}

void AAnimal::Server_MeleeAttack_Implementation()
{
	NetMulticast_PlayAttackAnimation();
}

void AAnimal::Server_InitAnimations_Implementation()
{
	if(HasAuthority())
	{
		if(NormalMontage && UpperBodyMontage)
		{
			const auto NotifyEventsNormal = NormalMontage->Notifies;
			const auto NotifyEventsUpper = UpperBodyMontage->Notifies;
			for(auto EventNotify : NotifyEventsNormal)
			{
				if(const auto AttackNotify = Cast<UAIAttackAnimNotifyState>(EventNotify.NotifyStateClass))
				{
					AttackNotify->AIAttackBegin.AddDynamic(this, &AAnimal::Server_OnAnimationBegin);
					AttackNotify->AIOnAttack.AddDynamic(this, &AAnimal::Server_OnAnimation);
					AttackNotify->AIAttackEnd.AddDynamic(this, &AAnimal::Server_OnAnimationFinish);
				}
			}
			for(auto EventNotify : NotifyEventsUpper)
			{
				if(const auto AttackNotify = Cast<UAIAttackAnimNotifyState>(EventNotify.NotifyStateClass))
				{
					AttackNotify->AIAttackBegin.AddDynamic(this, &AAnimal::Server_OnAnimationBegin);
					AttackNotify->AIOnAttack.AddDynamic(this, &AAnimal::Server_OnAnimation);
					AttackNotify->AIAttackEnd.AddDynamic(this, &AAnimal::Server_OnAnimationFinish);
				}
			}
		}
	}
}

void AAnimal::Server_ChangeMovementSpeed_Implementation(float NewSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

void AAnimal::Server_OnAnimationBegin_Implementation()
{
	if(MouthCollision)
	{
		MouthCollision->SetCollisionProfileName("Mouth", true);
	}
}

void AAnimal::Server_OnAnimation_Implementation()
{
	if(bIsAnimPlayed == false)
	{
		TArray<AActor*> OverlappingActors;
		MouthCollision->GetOverlappingActors(OverlappingActors);

		for(auto OverlappedActor : OverlappingActors)
		{
			if(OverlappedActor->FindComponentByClass<URPlayerStatComponent>())
			{
				URPlayerStatComponent* PlayerStatComp = Cast<URPlayerStatComponent>(
					OverlappedActor->FindComponentByClass<URPlayerStatComponent>());
				if(PlayerStatComp)
				{
					PlayerStatComp->DecreaseHealth(DamageAmount);
				}
				bIsAnimPlayed = true;
				break;
			}
		}
	}
}

void AAnimal::Server_OnAnimationFinish_Implementation()
{
	bIsAnimPlayed = false;

	if(MouthCollision)
	{
		MouthCollision->SetCollisionProfileName("Mouth", false);
	}
}

void AAnimal::NetMulticast_PlayAttackAnimation_Implementation()
{
	if(GetVelocity().Size() > 0)
	{
		PlayAnimMontage(UpperBodyMontage);
	}
	else
	{
		PlayAnimMontage(NormalMontage);
	}
}
