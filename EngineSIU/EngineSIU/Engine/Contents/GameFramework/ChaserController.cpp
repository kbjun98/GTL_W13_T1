#include "ChaserController.h"
#include "Engine/Contents/Actors/GridMapActor.h"
void AChaserController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    PathUpdateTimer += DeltaTime;
    if (PathUpdateTimer >= PathUpdateInterval)
    {
        PathUpdateTimer = 0.0f;
        UpdatePath();
    }

    if (IsPathValid())
    {
        if (!ControlledPawn)
        {
            ControlledPawn = Cast<AChasePawn>(ControlledActor);
        }

        if (CurrentPathIndex < CurrentPath.Num())
        {
            FGridNode* TargetNode = CurrentPath[CurrentPathIndex];
            FVector TargetLocation = FVector(0, 0, 0);
            ControlledPawn->MoveToTarget(TargetLocation, DeltaTime);

            // Target 도달 체크
            if (FVector::Dist(ControlledPawn->GetActorLocation(), TargetLocation) < CellTolerance)
            {
                CurrentPathIndex++;
            }
        }
    }
}

void AChaserController::StartChasing(const FVector& InDestination)
{
    Destination = InDestination;
    PathUpdateTimer = PathUpdateInterval;
}

void AChaserController::UpdatePath()
{
    if (!ControlledPawn) return;
    AGridMapActor* GridMapActor = GetControlledGridMapActor();

    int StartX = static_cast<int>(ControlledPawn->GetActorLocation().X);
    int StartY = static_cast<int>(ControlledPawn->GetActorLocation().Y);

    int TargetX = static_cast<int>(Destination.X);
    int TargetY = static_cast<int>(Destination.Y);

    FGridNode& Startnode = GridMapActor->GridComponent->GetNode(StartX, StartY);
}
