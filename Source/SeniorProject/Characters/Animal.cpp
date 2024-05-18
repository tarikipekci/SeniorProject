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
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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
	ChangeMovementSpeed(WalkSpeed);
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
	InitAnimations();
}

// Called every frame
void AAnimal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AAnimal::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

int AAnimal::MeleeAttack_Implementation()
{
	NetMulticast_PlayAttackAnimation();
	return 0;
}

void AAnimal::InitAnimations()
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
					AttackNotify->AIAttackBegin.AddDynamic(this, &AAnimal::OnAnimationBegin);
					AttackNotify->AIOnAttack.AddDynamic(this, &AAnimal::OnAnimation);
					AttackNotify->AIAttackEnd.AddDynamic(this, &AAnimal::OnAnimationFinish);
				}
			}
			for(auto EventNotify : NotifyEventsUpper)
			{
				if(const auto AttackNotify = Cast<UAIAttackAnimNotifyState>(EventNotify.NotifyStateClass))
				{
					AttackNotify->AIAttackBegin.AddDynamic(this, &AAnimal::OnAnimationBegin);
					AttackNotify->AIOnAttack.AddDynamic(this, &AAnimal::OnAnimation);
					AttackNotify->AIAttackEnd.AddDynamic(this, &AAnimal::OnAnimationFinish);
				}
			}
		}
	}
	else
	{
		Server_InitAnimations();
	}
}

void AAnimal::Server_InitAnimations_Implementation()
{
	InitAnimations();
}

void AAnimal::OnAnimationBegin()
{
	if(MouthCollision)
	{
		MouthCollision->SetCollisionProfileName("Mouth", true);
	}
}

void AAnimal::OnAnimation()
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

void AAnimal::OnAnimationFinish()
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

void AAnimal::ChangeMovementSpeed(float NewSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}
