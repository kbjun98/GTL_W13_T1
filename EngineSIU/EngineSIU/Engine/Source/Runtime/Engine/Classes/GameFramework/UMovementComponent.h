#pragma once
#include "Components/ActorComponent.h"
class UMovementComponent : public UActorComponent
{
    DECLARE_CLASS(UMovementComponent, UActorComponent)
public:
    UMovementComponent() = default;
    virtual UObject* Duplicate(UObject* InOuter) override;

    virtual void TickComponent(float DeltaTime) override;
private:
    FVector Velocity; // 현재 속도
};

