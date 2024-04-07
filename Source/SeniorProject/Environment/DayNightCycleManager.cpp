#include "DayNightCycleManager.h"

#include "Engine/DirectionalLight.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ADayNightCycleManager::ADayNightCycleManager()
{
	// Set this actor to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	// Set default sun rotation speed
	SunRotationSpeed = 10.0f;

	// Ensure replication is enabled for relevant variables
	bReplicates = true;
}

// Called when the game starts or when spawned
void ADayNightCycleManager::BeginPlay()
{
	Super::BeginPlay();

	if(GetLocalRole() == ROLE_Authority)
	{
		CurrentTimeInSeconds = 200.0f;
	}
}

// Called every frame
void ADayNightCycleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(GetLocalRole() == ROLE_Authority)
	{
		// Update time
		CurrentTimeInSeconds += DeltaTime;
		UpdateSunState();
	}
}

void ADayNightCycleManager::OnRep_CurrentTimeInSeconds()
{
	// Do nothing on clients when replicated
	UpdateSunState();
}

void ADayNightCycleManager::OnRep_TimeOfDay()
{
	// Do nothing on clients when replicated
}

void ADayNightCycleManager::UpdateSunState()
{
	if(CurrentTimeInSeconds >= DayDurationInSeconds)
	{
		CurrentTimeInSeconds -= DayDurationInSeconds;
	}

	// Calculate time of the day
	TimeOfDay = FMath::Fmod(CurrentTimeInSeconds / DayDurationInSeconds, 1.0f);

	// Rotate sun
	if(SunLight)
	{
		FRotator NewRotation = FRotator(TimeOfDay * 360.0f, 0.0f, 0.0f);
		SunLight->SetActorRotation(NewRotation);

		// Replicate sun rotation to all clients if we are a listen server
		if(GetNetMode() == NM_ListenServer)
		{
			SetSunRotation(NewRotation);
		}
	}
}

void ADayNightCycleManager::SetSunRotation_Implementation(const FRotator& NewRotation)
{
	if(SunLight)
	{
		SunLight->SetActorRotation(NewRotation);
	}
}

bool ADayNightCycleManager::SetSunRotation_Validate(const FRotator& NewRotation)
{
	return true;
}

void ADayNightCycleManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate these properties to all clients
	DOREPLIFETIME_CONDITION(ADayNightCycleManager, CurrentTimeInSeconds, COND_None);
	DOREPLIFETIME_CONDITION(ADayNightCycleManager, TimeOfDay, COND_None);
}
