#include "RabbitPawn.h"

void ARabbitPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    PlayerCam->Tick(DeltaTime);
}

void ARabbitPawn::PostSpawnInitialize()
{
    Super::PostSpawnInitialize();

    PlayerCam = std::make_shared<PlayerCamera>();
}

void ARabbitPawn::MoveForward(float DeltaTime)
{
    FVector ForwardXY = GetActorForwardVector();
    ForwardXY.Z = 0.0f; // Z축 이동을 무시하고 XY 평면에서만 이동
    FVector Delta = ForwardXY * MoveSpeed * DeltaTime;
    FRotator Rotation = GetActorRotation();
    GetRootComponent()->MoveComponent(Delta, Rotation, false);
}

void ARabbitPawn::MoveRight(float DeltaTime)
{
    FVector RightXY = GetActorRightVector();
    RightXY.Z = 0.0f; // Z축 이동을 무시하고 XY 평면에서만 이동
    FVector Delta = RightXY * MoveSpeed * DeltaTime;
    FRotator Rotation = GetActorRotation();
    GetRootComponent()->MoveComponent(Delta, Rotation, false);
}

void ARabbitPawn::RotateYaw(float DeltaTime)
{
    FRotator Rotation = GetActorRotation();
    Rotation.Yaw += RotateSpeed * DeltaTime;
    SetActorRotation(Rotation);
}

void ARabbitPawn::RotatePitch(float DeltaTime)
{
    FRotator Rotation = GetActorRotation();
    Rotation.Pitch = FMath::Clamp(Rotation.Pitch - RotateSpeed * DeltaTime, -89.0f, 89.0f);
    SetActorRotation(Rotation);
}

std::shared_ptr<PlayerCamera> ARabbitPawn::GetPlayerCamera()
{
    return PlayerCam;
}
