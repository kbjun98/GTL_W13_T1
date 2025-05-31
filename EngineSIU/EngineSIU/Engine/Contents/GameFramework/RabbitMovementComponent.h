#pragma once
#include "GameFramework/PawnMovementComponent.h"
#include "UObject/ObjectMacros.h"


class URabbitMovementComponent : public UPawnMovementComponent
{
    DECLARE_CLASS(URabbitMovementComponent, UPawnMovementComponent)

public:
    URabbitMovementComponent();
    virtual ~URabbitMovementComponent() override = default;

    virtual void TickComponent(float DeltaTime) override;

    virtual void PerformMovement(float DeltaTime);

protected:
    bool bGravity;
    float GravityFactor;
};
