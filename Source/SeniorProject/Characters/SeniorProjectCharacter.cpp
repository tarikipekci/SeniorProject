// Copyright Epic Games, Inc. All Rights Reserved.

#include "SeniorProjectCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TimerManager.h"
#include "Engine/DamageEvents.h"
#include "SeniorProject/Building/InventoryBuilding.h"
#include "SeniorProject/Components/RInteractComponent.h"
#include "SeniorProject/Components/RInventoryComponent.h"
#include "SeniorProject/Components/RLineTraceComponent.h"
#include "SeniorProject/Components/RPlayerStatComponent.h"
#include "SeniorProject/Environment/Item.h"
#include "SeniorProject/UI/RHUD.h"

//////////////////////////////////////////////////////////////////////////
// ASeniorProjectCharacter

ASeniorProjectCharacter::ASeniorProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create the player components
	PlayerStatComp = CreateDefaultSubobject<URPlayerStatComponent>("PlayerStatComponent");
	LineTraceComp = CreateDefaultSubobject<URLineTraceComponent>("LineTraceComponent");
	InteractComp = CreateDefaultSubobject<URInteractComponent>("InteractComponent");
	InventoryComp = CreateDefaultSubobject<URInventoryComponent>("InventoryComponent");
	PlayerNameTagComp = CreateDefaultSubobject<UWidgetComponent>("PlayerNameTagComponent");
	check(PlayerNameTagComp);
	PlayerNameTagComp->SetWidgetSpace(EWidgetSpace::Screen);
	PlayerNameTagComp->SetWorldLocation(FVector(0, 0, 0));
	PlayerNameTagComp->SetupAttachment(FollowCamera);

	InteractComp->SetupAttachment(RootComponent);
	bIsSprinting = false;
	StaminaDecrementTimerDuration = 0.1f;
	JumpStaminaCost = 25.0f;
	RespawnDuration = 5.0f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ASeniorProjectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if(APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	GetWorld()->GetTimerManager().SetTimer(SprintingHandle, this, &ASeniorProjectCharacter::HandleSprinting,
	                                       StaminaDecrementTimerDuration, true);
}

void ASeniorProjectCharacter::CallDestroy()
{
	this->Destroy();
}

float ASeniorProjectCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
                                          AController* EventInstigator, AActor* DamageCauser)
{
	if(GetLocalRole() < ROLE_Authority || PlayerStatComp->GetHealth() <= 0.0f)
	{
		return 0.0f;
	}

	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if(ActualDamage > 0.0f)
	{
		PlayerStatComp->DecreaseHealth(ActualDamage);
		if(PlayerStatComp->GetHealth() <= 0.0f)
		{
			Die();
		}
	}

	return ActualDamage;
}


//////////////////////////////////////////////////////////////////////////
// Input

void ASeniorProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this,
		                                   &ASeniorProjectCharacter::AttemptJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASeniorProjectCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASeniorProjectCharacter::Look);
	}
}

void ASeniorProjectCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if(Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);


		if(!bIsSprinting)
		{
			MovementVector.Y *= 0.5f;
			MovementVector.X *= 0.5f;
		}
		else
		{
			MovementVector.Y *= 1.0f;
			MovementVector.X *= 1.0f;
		}
		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASeniorProjectCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if(Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASeniorProjectCharacter::StartSprinting()
{
	if(PlayerStatComp->GetStamina() > 0 && GetVelocity().Size())
	{
		bIsSprinting = true;
		PlayerStatComp->ControlSprintingTimer(true);
	}
	else if(PlayerStatComp->GetStamina() <= 0.0f)
	{
		PlayerStatComp->ControlSprintingTimer(false);
	}
}

void ASeniorProjectCharacter::StopSprinting()
{
	bIsSprinting = false;
	PlayerStatComp->ControlSprintingTimer(false);
}

void ASeniorProjectCharacter::HandleSprinting()
{
	if(bIsSprinting && GetCharacterMovement()->IsFalling() == false)
	{
		PlayerStatComp->DecreaseStamina(PlayerStatComp->GetStaminaDecrementValue());
		if(PlayerStatComp->GetStamina() <= 0)
		{
			StopSprinting();
		}
	}
	if(this->GetVelocity().Size() <= 0)
	{
		StopSprinting();
	}
}

void ASeniorProjectCharacter::AttemptJump()
{
	if(PlayerStatComp)
	{
		if(PlayerStatComp->GetStamina() > JumpStaminaCost && !GetCharacterMovement()->IsFalling())
		{
			Jump();
			PlayerStatComp->DecreaseStamina(JumpStaminaCost);
		}
	}
}

void ASeniorProjectCharacter::Attack()
{
	FVector Start = InteractComp->GetComponentLocation();
	FVector End = Start + FollowCamera->GetForwardVector() * InteractComp->GetInteractRange();
	AActor* Actor = LineTraceComp->LineTraceSingle(Start, End, INTERACTABLE_CHANNEL, false);
	if(Actor)
	{
		ASeniorProjectCharacter* Player = Cast<ASeniorProjectCharacter>(Actor);
		if(Player)
		{
			Server_Attack(Player);
		}
	}
}

void ASeniorProjectCharacter::Server_Attack_Implementation(AActor* Actor)
{
	if(GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		FVector Start = InteractComp->GetComponentLocation();
		FVector End = Actor->GetActorLocation();
		AActor* HitActor = LineTraceComp->LineTraceSingle(Start, End, INTERACTABLE_CHANNEL, true);
		if(Actor)
		{
			ASeniorProjectCharacter* Player = Cast<ASeniorProjectCharacter>(HitActor);
			if(Player)
			{
				if(Player == Actor)
				{
					float TestDamage = 20.f;
					Player->TakeDamage(TestDamage, FDamageEvent(), GetController(), this);
				}
			}
		}
	}
}

void ASeniorProjectCharacter::Die()
{
	if(GetLocalRole() == ROLE_Authority)
	{
		//InventoryComp->DropAllInventoryItems();
		Multicast_Die();
		//Start destroy timer to remove player actor from world
		GetWorld()->GetTimerManager().SetTimer(DestroyHandle, this, &ASeniorProjectCharacter::CallDestroy,
		                                       RespawnDuration, false);
	}
}

void ASeniorProjectCharacter::Multicast_Die_Implementation()
{
	this->GetCharacterMovement()->DisableMovement();
	this->GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	this->GetMesh()->SetAllBodiesSimulatePhysics(true);
}

void ASeniorProjectCharacter::UsePickup(AItem* Item)
{
	if(Item)
	{
		if(HasAuthority())
		{
			Item->Use(this);
		}
		else
		{
			Server_UsePickup(Item);
		}
	}
}

void ASeniorProjectCharacter::Server_CloseInventoryBuilding_Implementation()
{
	AInventoryBuilding* LastInteractedInventoryBuilding = Cast<AInventoryBuilding>(InteractComp->GetInteractedActor());
	LastInteractedInventoryBuilding->SetIsOpened(false);
}

void ASeniorProjectCharacter::Server_UsePickup_Implementation(AItem* Item)
{
	for(FItemData ItemData : InventoryComp->GetInventoryItems())
	{
		if(ItemData.ItemActor == Item)
		{
			UsePickup(Item);
			return;
		}
	}
}

void ASeniorProjectCharacter::Client_OpenInventory_Implementation()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if(PlayerController)
	{
		AHUD* Hud = PlayerController->GetHUD();

		if(Hud)
		{
			if(ARHUD* ARHud = Cast<ARHUD>(Hud))
			{
				if(ARHud)
				{
					ARHud->PlayerInventoryWidget->SetVisibility(ESlateVisibility::Visible);
					ARHud->InteractableInventory->SetVisibility(ESlateVisibility::Visible);
					ARHud->InteractionWidget->SetRenderOpacity(0);
					PlayerController->SetIgnoreMoveInput(true);
					PlayerController->SetIgnoreLookInput(true);
					PlayerController->SetShowMouseCursor(true);
				}
			}
		}
	}
}

void ASeniorProjectCharacter::Client_CloseInventory_Implementation()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if(PlayerController)
	{
		AHUD* Hud = PlayerController->GetHUD();

		if(Hud)
		{
			if(ARHUD* ARHud = Cast<ARHUD>(Hud))
			{
				if(ARHud)
				{
					ARHud->InteractableInventory->SetVisibility(ESlateVisibility::Hidden);
					Server_CloseInventoryBuilding();
				}
			}
		}
	}
}
