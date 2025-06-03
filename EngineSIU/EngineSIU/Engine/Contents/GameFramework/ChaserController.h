#pragma once

#include "GameFramework/AIController.h"

class ARabbitEnemy;
class ARabbitPlayer;

class AChaserController : public AAIController
{
    DECLARE_CLASS(AChaserController, AAIController)

public:
    AChaserController() = default;

    virtual void Tick(float DeltaTime) override;
    virtual void UpdatePath() override;

    ARabbitPlayer* GetTargetRabbitPlayer();    
    ARabbitEnemy* GetPossesedRabbitEnemy();
private:
    float PathUpdateInterval = 0.5f;
    float PathUpdateTimer = 0.0f;

};
