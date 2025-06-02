#pragma once
#include "GameFramework/AIController.h"

class ARabbitEnemy;
class ARabbitPlayer;

enum class EnemyState : uint8
{
    IDLE,
    CHASE,
    END
};

class ARabbitEnemyController : public AAIController
{
    DECLARE_CLASS(ARabbitEnemyController, AAIController)
public:
    ARabbitEnemyController() = default;
    virtual void PostSpawnInitialize() override;
    virtual void Tick(float DeltaTime) override;

public:
    void ProcessEnemyMovement(float DeltaTime);
    void CheckStateChange();
    bool IsTargetInFOV();
    bool IsTargetInRange(float Min, float Max);
    bool IsTargetInSight(float Min, float Max);

public:
    ARabbitPlayer* GetTargetRabbitPlayer();
    ARabbitEnemy* GetPossesedRabbitEnemy();

    bool IsChasing = true;
private:
    void MoveTo(const FVector& TargetLocation);
private:
    EnemyState CurrentState = EnemyState::IDLE; // 현재 상태
    UPROPERTY(EditAnywhere,
        float, SightFOV ,= 30.0f;
    )
    UPROPERTY(EditAnywhere,
    float ,AcceptanceRadius, = 50.0f; // 목표 위치에 도달하기 위한 허용 반경
    )
    UPROPERTY(EditAnywhere,
    float ,FailureRadius ,= 500.0f; // 추적 실패 반경
    )
    UPROPERTY(EditAnywhere,
    float ,ChaseRangeMax ,= 500.0f;
    )
    UPROPERTY(EditAnywhere,
    float, ChaseRangeMin ,= 200.0f; // 추적 범위
    )
};

