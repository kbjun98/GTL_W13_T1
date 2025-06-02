#pragma once
#include "GameFramework/Pawn.h"
#include "Core/Delegates/Delegate.h"
class AChasePawn;

FUNC_DECLARE_MULTICAST_DELEGATE(FOnDestroyedSignature, void, AChasePawn*);

class AChasePawn : public APawn
{
    DECLARE_CLASS(AChasePawn, APawn)

public:

    AChasePawn() = default;
    
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    void SetTargetLocation(const FVector& NewTarget);

private:
    FVector TargetLocation;
    float MoveSpeed = 100.0f;

public:
    FOnDestroyedSignature OnDestroyed;

    virtual bool Destroy() override;
};

