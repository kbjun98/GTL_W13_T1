#include "RabbitPawn.h"

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

    PlayerCam = std::make_shared<RabbitCamera>();
}

UObject* ARabbitPawn::Duplicate(UObject* InOuter)
{
    ARabbitPawn* NewPawn = Cast<ARabbitPawn>(Super::Duplicate(InOuter));
    NewPawn->PlayerCam = std::make_shared<RabbitCamera>();
    
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
            float NewPitch = ControlRotation.Pitch;
            if (APlayerCameraManager* CameraManager = Controller->PlayerCameraManager)
            {
                NewPitch = FMath::Clamp(
                    NewPitch,
                    CameraManager->ViewPitchMin,
                    CameraManager->ViewPitchMax
                );
            }
            Camera->SetRelativeRotation(FRotator(NewPitch, 0.f, 0.f));
        }
    }
}

std::shared_ptr<RabbitCamera> ARabbitPawn::GetPlayerCamera()
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
