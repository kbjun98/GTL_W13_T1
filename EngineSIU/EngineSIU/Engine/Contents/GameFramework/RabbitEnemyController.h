#pragma once
#include "GameFramework/AIController.h"

class ARabbitEnemy;
class ARabbitPlayer;

class ARabbitEnemyController : public AAIController
{
    DECLARE_CLASS(ARabbitEnemyController, AAIController)
public:
    ARabbitEnemyController() = default;
    virtual void Tick(float DeltaTime) override;
    void ProcessEnemyMovement(float DeltaTime);
public:
    ARabbitPlayer* GetTargetRabbitPlayer();
    ARabbitEnemy* GetPossesedRabbitEnemy();

    bool IsChasing = true;
private:
    void MoveTo(const FVector& TargetLocation);
};

