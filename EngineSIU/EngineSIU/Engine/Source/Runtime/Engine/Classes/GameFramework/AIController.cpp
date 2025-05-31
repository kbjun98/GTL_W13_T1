#include "AIController.h"
#include "Engine/Contents/Actors/GridMapActor.h"
#include "Engine/Contents/Navigation/PathFinder.h"
#include "Engine/Contents/Navigation/MapData.h"
#include "Engine/Contents/Components/GridMapComponent.h"

void AAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (IsPathValid())
    {
        MoveAlongPath(DeltaTime);
    }
}

void AAIController::MoveTo(const FVector& InDestination)
{
    Destination = InDestination;
    AGridMapActor* GridMapActor = GetControlledGridMapActor();

    int StartX = static_cast<int>(GridMapActor->GetActorLocation().X);
    int StartY = static_cast<int>(GridMapActor->GetActorLocation().Y);

    int TargetX = static_cast<int>(Destination.X);
    int TargetY = static_cast<int>(Destination.Y);

    FGridNode& StartNode = GridMapActor->GridComponent->GetNode(StartX,StartY);
    FGridNode& TargetNode = GridMapActor->GridComponent->GetNode(TargetX, TargetY);

    FPathFinder* PathFinder = GridMapActor->GridComponent->PathFinder;
    //CurrentPath = PathFinder->FindPath(GridMapActor->GridComponent->GridMap, StartNode, TargetNode);

    if (CurrentPath.IsEmpty())
    {
        UE_LOG(ELogLevel::Display, "No path found to destination.");
    }
    else
    {
        CurrentPathIndex = 0;
        UE_LOG(ELogLevel::Display, "Path found. Starting to move.");
    }
}

void AAIController::OnMoveCompleted()
{
}

void AAIController::MoveAlongPath(float DeltaTime)
{
    if (CurrentPathIndex >= CurrentPath.Num())
    {
        UE_LOG(ELogLevel::Display, "Reached the end of the path.");
        return;
    }

    AGridMapActor* GridMapActor = GetControlledGridMapActor();

    FGridNode* TargetNode = CurrentPath[CurrentPathIndex];
    FVector TargetLocation = GridMapActor->GridComponent->GridMap.GetWorldPosition(TargetNode->X, TargetNode->Y);

    FVector CurrentLocation = GridMapActor->GetActorLocation();
    FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
    FVector NewLocation = CurrentLocation + Direction * MovementSpeed * DeltaTime;

    GridMapActor->SetActorLocation(NewLocation);

    if (FVector::Dist(NewLocation, TargetLocation) < CellTolerance) // 10 units tolerance
    {
        CurrentPathIndex++;
        if (CurrentPathIndex >= CurrentPath.Num())
        {
            OnMoveCompleted();
        }
    }
}

bool AAIController::IsPathValid() const
{
    return CurrentPath.Num() > 0 && CurrentPathIndex < CurrentPath.Num();
}

AGridMapActor* AAIController::GetControlledGridMapActor() const
{
    return Cast<AGridMapActor>(ControlledActor);
}
