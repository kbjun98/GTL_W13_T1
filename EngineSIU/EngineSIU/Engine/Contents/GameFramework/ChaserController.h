#pragma once

#include "GameFramework/AIController.h"
#include "ChasePawn.h"

class AChasePawn;
class AChaserController : public AAIController
{
    DECLARE_CLASS(AChaserController, AAIController)

public:
    AChaserController() = default;

    virtual void Tick(float DeltaTime) override;
    virtual void UpdatePath() override;

    void SetChasePawn(AChasePawn* InPawn);
private:
    float PathUpdateInterval = 0.5f;
    float PathUpdateTimer = 0.0f;

    AChasePawn* ChasePawn = nullptr;
};
