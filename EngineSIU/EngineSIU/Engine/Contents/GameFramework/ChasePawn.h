#pragma once
#include "GameFramework/Pawn.h"
class AChasePawn : public APawn
{
    DECLARE_CLASS(AChasePawn, APawn)

public:
    AChasePawn() = default;
    
    virtual void Tick(float DeltaTime) override;

    void MoveToTarget(const FVector& TargetLocation, float DeltaTime);
    void SetMoveSpeed(float Speed);

private:
    float MoveSpeed = 100.0f;
};
