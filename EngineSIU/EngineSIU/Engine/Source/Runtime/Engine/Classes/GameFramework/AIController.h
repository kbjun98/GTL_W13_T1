#pragma once
#include "Actor.h"
#include "Classes/GameFramework/Controller.h"

struct FGridNode;
class AGridMapActor;
class FGridMap;
class FPathFinder;

class AAIController : public AController
{
    DECLARE_CLASS(AAIController, AActor)
public:
    AAIController() = default;
    virtual ~AAIController() = default;
    virtual void PostSpawnInitialize() override;
    virtual UObject* Duplicate(UObject* InOuter) override;
    virtual void Tick(float DeltaTime) override;

    // 경로 갱신 (ex : 플레이어 추적, 순찰 경로 등)
    virtual void UpdatePath();

    virtual void OnMoveCompleted();

protected:
    //FGridMap* GridMap;

    FPathFinder* PathFinder;

    TArray<FVector> CurrentPath; // 경로를 저장하는 배열
    
    int32 CurrentPathIndex = 0; // 현재 경로의 인덱스
    
    UPROPERTY(EditAnywhere,
        float, MovementSpeed, = 1.0f; // 이동 속도
    )
    float AcceptanceRadius = 5.0f;

    bool bMoving = false;

    APawn* TargetPawn = nullptr; // 이동할 대상 Pawn

    //float CellTolerance = 1.0f;
protected:
    void MoveAlongPath(float DeltaTime);
    FVector GetNextLocation();
    bool IsPathValid() const;   

private:    
    float PathUpdateInterval = 1.0f;
    float PathUpdateTimer = 0.0f;

};

