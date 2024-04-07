#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DayNightCycleManager.generated.h"

class ADirectionalLight;

UCLASS()
class SENIORPROJECT_API ADayNightCycleManager : public AActor
{
    GENERATED_BODY()

public: 
    // Sets default values for this actor's properties
    ADayNightCycleManager();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public: 
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // The total duration of a full day in seconds
    UPROPERTY(EditAnywhere, Category = "Day Night Cycle")
    float DayDurationInSeconds;

    // Current time in seconds
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CurrentTimeInSeconds, Category = "Day Night Cycle")
    float CurrentTimeInSeconds;

    // Current time of the day (0-1 where 0 is midnight, 0.5 is noon, 1 is midnight again)
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_TimeOfDay, Category = "Day Night Cycle")
    float TimeOfDay;

    // Sun actor to rotate
    UPROPERTY(EditAnywhere, Category = "Day Night Cycle")
    ADirectionalLight* SunLight;

    // Sun rotation speed
    UPROPERTY(EditAnywhere, Category = "Day Night Cycle")
    float SunRotationSpeed;

    // Replicated function to update CurrentTimeInSeconds on clients
    UFUNCTION()
    void OnRep_CurrentTimeInSeconds();

    // Replicated function to update TimeOfDay on clients
    UFUNCTION()
    void OnRep_TimeOfDay();

    // Replicated function to set sun rotation on clients
    UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
    void SetSunRotation(const FRotator& NewRotation);

    UFUNCTION()
    void UpdateSunState();
};