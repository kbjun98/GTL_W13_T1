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
    void RotateToTarget(const FVector& Location, float DeltaTime);
    
public:
    UPROPERTY(EditAnywhere,
        float, SightFOV, = 30.0f;
    )
    UPROPERTY(EditAnywhere,
        float, AcceptanceRadius, = 50.0f; // 목표 위치에 도달하기 위한 허용 반경
    )
    UPROPERTY(EditAnywhere,
        float, FailureRadius, = 500.0f; // 추적 실패 반경
    )
    UPROPERTY(EditAnywhere,
        float, ChaseRangeMax, = 1000.0f;
    )
    UPROPERTY(EditAnywhere,
        float, ChaseRangeMin, = 200.0f; // 추적 범위
    )
public:
    TArray<USceneComponent*> PatrolTargets;

private:
    FVector TargetLocation = FVector::ZeroVector; // 적의 목표 위치
    const uint32 PatrolTargetNum = 2;



};

