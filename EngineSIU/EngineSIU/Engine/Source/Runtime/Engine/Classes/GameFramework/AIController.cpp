#include "AIController.h"
#include "Engine/Contents/Actors/GridMapActor.h"
#include "Engine/Contents/Navigation/PathFinder.h"
#include "Engine/Contents/Navigation/MapData.h"
#include "Engine/Contents/Components/GridMapComponent.h"
#include "Classes/GameFramework/Pawn.h"
#include <Engine/EditorEngine.h>
void AAIController::PostSpawnInitialize()
{
    Super::PostSpawnInitialize();
    PathFinder = new FPathFinder();
}

UObject* AAIController::Duplicate(UObject* InOuter)
{
    ThisClass* NewComponent = Cast<ThisClass>(Super::Duplicate(InOuter));
    return NewComponent;
}

void AAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    PathUpdateTimer += DeltaTime;
    if (PathUpdateTimer >= PathUpdateInterval)
    {
        PathUpdateTimer = 0.0f;
        //UpdatePath();
    }
}

void AAIController::UpdatePath()
{
    
    FGridMap* GridMap = GEngine->ActiveWorld->GetGridMap();
    if (TargetPawn && PossessedPawn)
    {
        FVector StartLocation = PossessedPawn->GetActorLocation();
        FVector TargetLocation = TargetPawn->GetActorLocation();;

        CurrentPath = PathFinder->FindWorlPosPathByWorldPos(
            *GridMap, StartLocation, TargetLocation);
        CurrentPathIndex = 0;
    }
    else
    {
        CurrentPath.Empty();
    }
}

void AAIController::OnMoveCompleted()
{
    bMoving = false;
}

void AAIController::MoveAlongPath(float DeltaTime)
{
    if (CurrentPathIndex >= CurrentPath.Num())
    {
        OnMoveCompleted();
        //UE_LOG(ELogLevel::Display, "Reached the end of the path.");
        return;
    }
    // FIXME GridNode에서 Location Vector리턴하도록 책임 변경.
    // TODO : bMoving = true 조건 전환 타이밍 수정하기
    bMoving = true;
    FVector TargetLocation = PossessedPawn->GetActorLocation();
    FVector CurrentLocation = GetActorLocation();
    FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
    FVector NewLocation = CurrentLocation + Direction * MovementSpeed * DeltaTime;
    
    SetActorLocation(NewLocation);

    if (FVector::Dist(NewLocation, TargetLocation) < AcceptanceRadius)
    {
        CurrentPathIndex++;
        if (CurrentPathIndex >= CurrentPath.Num())
        {
            // 경로 완료 시 처리
            OnMoveCompleted();
        }
    }
}

FVector AAIController::GetNextLocation()
{
    FVector TargetDirection = CurrentPath[CurrentPathIndex] - PossessedPawn->GetActorLocation();
    TargetDirection.Z = 0.0f; // Z축 방향은 무시
    FVector DirNormal = TargetDirection.GetSafeNormal();
    if (DirNormal.Size() <0.01f)
    {
        CurrentPathIndex++;
        //UE_LOG(ELogLevel::Error, "CurrentPathIndex : %d", CurrentPathIndex);
    }
    if (CurrentPathIndex < CurrentPath.Num())
    {
        return CurrentPath[CurrentPathIndex];
    }
    else {
        //UE_LOG(ELogLevel::Error, "Invalid currentPathIndex");
        return FVector::ZeroVector;
    }
    
}

bool AAIController::IsPathValid() const
{
    return CurrentPath.Num() > 0 && CurrentPathIndex < CurrentPath.Num();
}
