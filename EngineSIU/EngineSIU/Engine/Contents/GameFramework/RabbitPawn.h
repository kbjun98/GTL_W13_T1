#pragma once
#include "GameFramework/Pawn.h"

class URabbitMovementComponent;
class RabbitCamera;
class USkeletalMeshComponent;

class ARabbitPawn : public APawn
{
    DECLARE_CLASS(ARabbitPawn, APawn)
    
public:
    ARabbitPawn() = default;

    virtual void PostSpawnInitialize() override;

    virtual UObject* Duplicate(UObject* InOuter) override;

    virtual void BeginPlay() override;
    
    virtual void Tick(float DeltaTime) override;
    
    virtual FVector GetActorForwardVector() const override;
    virtual FVector GetActorRightVector() const override;
    
    int32 GetMaxHealth() const { return MaxHealth; }
    void SetMaxHealth(int32 Value);
    
    int32 GetCurrentHealth() const { return CurrentHealth; }
    void SetCurrentHealth(int32 Value);

private:
    URabbitMovementComponent* MovementComponent;
    USkeletalMeshComponent* SkeletalMeshComp;

    float HalfHeight = 40.f;
    float Radius = 20.f;

    float RotateSpeed = 100.0f;
    
    UPROPERTY(EditAnywhere, int32, MaxHealth, = 100)
    UPROPERTY(EditAnywhere, int32, CurrentHealth, = 100)

};

