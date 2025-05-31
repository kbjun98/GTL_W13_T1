#pragma once
#include "GameFramework/Pawn.h"

class URabbitMovementComponent;

class ARabbitPawn : public APawn
{
    DECLARE_CLASS(ARabbitPawn, APawn)
    
public:
    ARabbitPawn() = default;

    virtual void PostSpawnInitialize() override;
    
    virtual void Tick(float DeltaTime) override;
    
private:
    URabbitMovementComponent* MovementComponent;

    float HalfHeight = 40.f;
};

