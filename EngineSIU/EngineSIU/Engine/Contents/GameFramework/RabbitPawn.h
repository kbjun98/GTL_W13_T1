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
    
private:
    URabbitMovementComponent* MovementComponent;

    float HalfHeight = 40.f;
    float Radius = 30.f;
};

