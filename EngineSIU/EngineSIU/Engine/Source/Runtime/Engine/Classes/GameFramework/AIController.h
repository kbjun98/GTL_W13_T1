#pragma once
#include "Actor.h"

class FGridNode;
class AGridMapActor;
class AAIController : public AActor
{
    DECLARE_CLASS(AAIController, AActor)
public:
    AAIController() = default;
    virtual ~AAIController() = default;
    virtual void Tick(float DeltaTime) override;
    virtual void MoveTo(const FVector& InDestination);
    virtual void OnMoveCompleted();
protected:
    FVector Destination;
    TArray<FGridNode*> CurrentPath; // 경로를 저장하는 배열
    int32 CurrentPathIndex = 0; // 현재 경로의 인덱스

protected:
    void MoveAlongPath(float DeltaTime);
    bool IsPathValid() const;

protected:
    AActor* ControlledActor = nullptr; // 이 AI가 제어하는 Actor
    AGridMapActor* GetControlledGridMapActor() const;
protected:
    float MovementSpeed = 100.0f; // 이동 속도
    float CellTolerance = 1.0f;
};

