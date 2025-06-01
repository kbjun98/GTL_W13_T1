#pragma once
#include "GameFramework/PawnMovementComponent.h"
#include "UObject/ObjectMacros.h"


namespace physx
{
    class PxController;
}

class URabbitMovementComponent : public UPawnMovementComponent
{
    DECLARE_CLASS(URabbitMovementComponent, UPawnMovementComponent)

public:
    URabbitMovementComponent();
    virtual ~URabbitMovementComponent() override = default;

    virtual void BeginPlay() override;
    
    virtual void SetUpdatedComponent(USceneComponent* NewUpdatedComponent) override;

    virtual void DestroyComponent(bool bPromoteChildren) override;
    
    virtual void TickComponent(float DeltaTime) override;

    virtual void PerformMovement(float DeltaTime);

protected:
    bool bGravity;
    float GravityFactor;

    physx::PxController* Controller;
};
