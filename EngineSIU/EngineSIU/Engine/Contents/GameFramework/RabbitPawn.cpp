#include "RabbitPawn.h"

#include "PhysicsManager.h"
#include "Camera/CameraComponent.h"
#include "RabbitMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PlayerController.h"
#include "Math/JungleMath.h"

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
    NewPawn->PlayerCam = std::make_shared<PlayerCamera>();
    
    return NewPawn;
}

void ARabbitPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (PlayerCam)
    {
        PlayerCam->Tick(DeltaTime);
    }
    
    if (Controller)
    {
        const FRotator& ControlRotation = Controller->GetControlRotation();
        SetActorRotation(FRotator(0.f, ControlRotation.Yaw, 0.f));

        if (UCameraComponent* Camera = GetComponentByClass<UCameraComponent>())
        {
            Camera->SetRelativeRotation(FRotator(ControlRotation.Pitch, 0.f, 0.f));
        }
    }
}

std::shared_ptr<PlayerCamera> ARabbitPawn::GetPlayerCamera()
{
    return PlayerCam;
}

FVector ARabbitPawn::GetActorForwardVector() const
{
    if (Controller)
    {
        const FRotator& ControlRotation = Controller->GetControlRotation();
        FRotator ActualRotation = FRotator(0.f, ControlRotation.Yaw, 0.f);
        return ActualRotation.ToVector();
    }
    return APawn::GetActorForwardVector();
}

FVector ARabbitPawn::GetActorRightVector() const
{
    if (Controller)
    {
        const FRotator& ControlRotation = Controller->GetControlRotation();
        FRotator ActualRotation = FRotator(0.f, ControlRotation.Yaw, 0.f);
        
        FVector Right = FVector::RightVector;
        Right = JungleMath::FVectorRotate(Right, ActualRotation);
        return Right;
    }
    return APawn::GetActorRightVector();
}

void ARabbitPawn::Jump()
{
    if (URabbitMovementComponent* RabbitMoveComp = Cast<URabbitMovementComponent>(GetMovementComponent()))
    {
        RabbitMoveComp->Jump();
    }
}
