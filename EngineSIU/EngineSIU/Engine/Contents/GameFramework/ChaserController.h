#include "GameFramework/AIController.h"
#include "ChasePawn.h"
#pragma once
class AChaserController : public AAIController
{
    DECLARE_CLASS(AChaserController, AAIController)

public:
    AChaserController() = default;

    virtual void Tick(float DeltaTime) override;
    void StartChasing(const FVector& InDestination);

private:
    void UpdatePath();                      // 일정시간마다 호출될 경로 갱신
   

private:
    // Pawn 캐싱
    AChasePawn* ControlledPawn = nullptr;

    float PathUpdateInterval = 1.0f;        // 1초마다 경로를 갱신
    float PathUpdateTimer = 0.0f;
};
