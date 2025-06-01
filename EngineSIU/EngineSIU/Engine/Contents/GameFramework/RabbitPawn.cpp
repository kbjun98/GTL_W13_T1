#include "RabbitPawn.h"

#include "PhysicsManager.h"
#include "Camera/CameraComponent.h"
#include "RabbitMovementComponent.h"
#include "Components/CapsuleComponent.h"

void ARabbitPawn::PostSpawnInitialize()
{
    APawn::PostSpawnInitialize();

    UCapsuleComponent* Collision = AddComponent<UCapsuleComponent>("Collision_0");
    Collision->SetHalfHeight(HalfHeight);
    Collision->SetRadius(Radius);
    RootComponent = Collision;
    
    UCameraComponent* Camera = AddComponent<UCameraComponent>("Camera_0");
    Camera->SetupAttachment(RootComponent);
    
    MovementComponent = AddComponent<URabbitMovementComponent>("RabbitMoveComp_0");

    PlayerCam = std::make_shared<PlayerCamera>();
}

UObject* ARabbitPawn::Duplicate(UObject* InOuter)
{
    ARabbitPawn* NewPawn = Cast<ARabbitPawn>(Super::Duplicate(InOuter));
    
    return NewPawn;
}

void ARabbitPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (PlayerCam)
    {
        PlayerCam->Tick(DeltaTime);
    }
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
