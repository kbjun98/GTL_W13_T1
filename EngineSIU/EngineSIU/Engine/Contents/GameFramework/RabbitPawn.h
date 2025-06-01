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
    
    virtual void Tick(float DeltaTime) override;

    std::shared_ptr<RabbitCamera> GetPlayerCamera();

    virtual FVector GetActorForwardVector() const override;
    virtual FVector GetActorRightVector() const override;

    void Jump();

private:
    URabbitMovementComponent* MovementComponent;

    float HalfHeight = 40.f;
    float Radius = 30.f;

    float RotateSpeed = 100.0f;

    std::shared_ptr<RabbitCamera> RabbitCam= nullptr;
};

