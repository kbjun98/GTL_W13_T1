#include "ChasePawn.h"

void AChasePawn::Tick(float DeltaTime)
{
}

void AChasePawn::MoveToTarget(const FVector& TargetLocation, float DeltaTime)
{
    FVector CurrentLocation = GetActorLocation();
    FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
    FVector NewLocation = CurrentLocation + Direction * MoveSpeed * DeltaTime;

    SetActorLocation(NewLocation);
}
