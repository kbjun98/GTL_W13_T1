#include "RabbitPawn.h"

#include "PhysicsManager.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "RabbitMovementComponent.h"

void ARabbitPawn::PostSpawnInitialize()
{
    APawn::PostSpawnInitialize();

    USphereComponent* Collision = AddComponent<USphereComponent>("Collision_0");
    Collision->SetRadius(HalfHeight);
    RootComponent = Collision;
    
    UCameraComponent* Camera = AddComponent<UCameraComponent>("Camera_0");
    Camera->SetupAttachment(RootComponent);
    
    MovementComponent = AddComponent<URabbitMovementComponent>("RabbitMoveComp_0");
}

void ARabbitPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
