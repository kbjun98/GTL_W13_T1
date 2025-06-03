#pragma once
#include "GameFramework/AIController.h"

class ARabbitEnemy;
class ARabbitPlayer;

enum class EnemyState : uint8
{
    IDLE,
    CHASE,
    ATTACK,
    PATROL,
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
    void CheckStateChange(float DeltaTime);
    bool IsTargetInFOV(float FOV);
    bool IsTargetInRange(float Min, float Max);
    bool IsTargetInSight(float Min, float Max);

public:
    ARabbitPlayer* GetTargetRabbitPlayer();
    ARabbitEnemy* GetPossesedRabbitEnemy();

    uint32 PatrolTargetIndex;

    void UpdatePatrolPath();
    bool IsChasing = true;
private:
    void MoveTo(const FVector& TargetLocation, float DeltaTime);
private:
    EnemyState CurrentState = EnemyState::IDLE; // 현재 상태
    uint32 CurPatrolIndex = 0;
    float IdleTime = 0.0f;
};

