#pragma once
#include "RabbitPawn.h"
class ARabbitEnemy : public ARabbitPawn
{
    DECLARE_CLASS(ARabbitEnemy, ARabbitPawn)
public:
    ARabbitEnemy() = default;
    virtual void PostSpawnInitialize() override;
    virtual UObject* Duplicate(UObject* InOuter) override;
    virtual void Tick(float DeltaTime) override;
    virtual void BeginPlay() override;
public:
    void SetTargetLocation(const FVector& Location) { TargetLocation = Location; }
    void RoatateToTarget(const FVector& Location);
private:
    FVector TargetLocation = FVector::ZeroVector; // 적의 목표 위치
    float AcceptanceRadius = 10.0f; // 목표 위치에 도달하기 위한 허용 반경
};

