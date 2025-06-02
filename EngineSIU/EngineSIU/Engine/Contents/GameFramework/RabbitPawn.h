#pragma once
#include "GameFramework/Pawn.h"

class URabbitMovementComponent;

class ARabbitPawn : public APawn
{
    DECLARE_CLASS(ARabbitPawn, APawn)
    
public:
    ARabbitPawn() = default;

    virtual void PostSpawnInitialize() override;

    virtual UObject* Duplicate(UObject* InOuter) override;

    virtual void BeginPlay() override;
    
    virtual void Tick(float DeltaTime) override;

    std::shared_ptr<RabbitCamera> GetPlayerCamera();

    virtual FVector GetActorForwardVector() const override;
    virtual FVector GetActorRightVector() const override;

    void Jump();

    int32 GetMaxHealth() const { return MaxHealth; }
    void SetMaxHealth(int32 Value);
    
    int32 GetCurrentHealth() const { return CurrentHealth; }
    void SetCurrentHealth(int32 Value);

private:
    URabbitMovementComponent* MovementComponent;

    float HalfHeight = 40.f;
    float Radius = 30.f;


    float RotateSpeed = 100.0f;

    std::shared_ptr<PlayerCamera> PlayerCam = nullptr;

    UPROPERTY(EditAnywhere, int32, MaxHealth, = 100)
    UPROPERTY(EditAnywhere, int32, CurrentHealth, = 100)

};

