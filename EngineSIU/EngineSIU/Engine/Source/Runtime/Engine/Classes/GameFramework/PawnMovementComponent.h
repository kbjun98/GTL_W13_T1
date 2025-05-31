#pragma once
#include "UMovementComponent.h"
class UPawnMovementComponent : public UMovementComponent
{
    DECLARE_CLASS(UPawnMovementComponent, UMovementComponent)
public:
    UPawnMovementComponent() = default;
    virtual UObject* Duplicate(UObject* InOuter) override;
    virtual void TickComponent(float DeltaTime) override;
};

