#include "RabbitPlayer.h"
#include "GameFramework/PlayerController.h"
#include "Math/JungleMath.h"
#include "RabbitMovementComponent.h"

void ARabbitPlayer::PostSpawnInitialize()
{
    Super::PostSpawnInitialize();
    
    UCameraComponent* Camera = AddComponent<UCameraComponent>("Camera_0");
    Camera->SetupAttachment(RootComponent);

    RabbitCam = std::make_shared<RabbitCamera>();
}

void ARabbitPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (RabbitCam)
    {
        RabbitCam->Tick(DeltaTime);
    }

    if (GetPlayerController())
    {
        const FRotator& ControlRotation = GetPlayerController()->GetControlRotation();
        SetActorRotation(FRotator(0.f, ControlRotation.Yaw, 0.f));

        if (UCameraComponent* Camera = GetComponentByClass<UCameraComponent>())
        {
            float NewPitch = ControlRotation.Pitch;
            if (APlayerCameraManager* CameraManager = GetPlayerController()->PlayerCameraManager)
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

UObject* ARabbitPlayer::Duplicate(UObject* InOuter)
{
    ARabbitPlayer* NewPawn = Cast<ARabbitPlayer>(Super::Duplicate(InOuter));
    RabbitCam = std::make_shared<RabbitCamera>();
    return NewPawn;
}

FVector ARabbitPlayer::GetActorForwardVector() const
{
    if (GetPlayerController())
    {
        const FRotator& ControlRotation = GetPlayerController()->GetControlRotation();
        FRotator ActualRotation = FRotator(0.f, ControlRotation.Yaw, 0.f);
        return ActualRotation.ToVector();
    }
    return Super::GetActorForwardVector();
}

FVector ARabbitPlayer::GetActorRightVector() const
{
    if (GetPlayerController())
    {
        const FRotator& ControlRotation = GetPlayerController()->GetControlRotation();
        FRotator ActualRotation = FRotator(0.f, ControlRotation.Yaw, 0.f);

        FVector Right = FVector::RightVector;
        Right = JungleMath::FVectorRotate(Right, ActualRotation);
        return Right;
    }
    return Super::GetActorRightVector();
}

std::shared_ptr<RabbitCamera> ARabbitPlayer::GetRabbitCamera()
{
    return RabbitCam;
}


void ARabbitPlayer::Jump()
{
    if (URabbitMovementComponent* RabbitMoveComp = Cast<URabbitMovementComponent>(GetMovementComponent()))
    {
        RabbitMoveComp->Jump();
    }
}
