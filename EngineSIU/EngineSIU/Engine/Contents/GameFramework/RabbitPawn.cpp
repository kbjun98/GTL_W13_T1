#include "RabbitPawn.h"

void ARabbitPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ARabbitPawn::MoveForward(float DeltaTime)
{
    FVector Delta = FVector(1, 0, 0) * MoveSpeed * DeltaTime;
    FRotator Rotation = GetActorRotation();
    GetRootComponent()->MoveComponent(Delta, Rotation, false);
}

void ARabbitPawn::MoveRight(float DeltaTime)
{
    FVector Delta = FVector(0, 1, 0) * MoveSpeed * DeltaTime;
    FRotator Rotation = GetActorRotation();
    GetRootComponent()->MoveComponent(Delta, Rotation, false);
}
