#include "RabbitPlayer.h"
#include "GameFramework/PlayerController.h"
#include "Math/JungleMath.h"
#include "RabbitMovementComponent.h"
#include "Camera/CameraShakeBase.h"
#include "Components/CameraMeshComponent.h"

void ARabbitPlayer::PostSpawnInitialize()
{
    Super::PostSpawnInitialize();
    
    UCameraComponent* Camera = AddComponent<UCameraComponent>("Camera_0");
    Camera->SetupAttachment(RootComponent);

    CameraMesh = AddComponent<UCameraMeshComponent>("CameraMesh_0");
    CameraMesh->SetupAttachment(Camera);

    RabbitCam = std::make_shared<RabbitCamera>();
}

void ARabbitPlayer::BeginPlay()
{
    Super::BeginPlay();

    if (GetPlayerController())
    {
        GetPlayerController()->PlayerCameraManager->DefaultFOV = DefaultFOV;
    }
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
    NewPawn->RabbitCam = std::make_shared<RabbitCamera>();
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

void ARabbitPlayer::ZoomIn(float DeltaTime)
{
    if (!IsADS())
    {
        return;
    }

    const float CurrentFOV = GetFOV();
    float NextFOV = CurrentFOV - FOVChangeSpeed * DeltaTime;
    NextFOV = FMath::Clamp(NextFOV, MinFOV_ADS, MaxFOV_ADS);
    SetFOV(NextFOV);
}

void ARabbitPlayer::ZoomOut(float DeltaTime)
{
    if (!IsADS())
    {
        return;
    }

    const float CurrentFOV = GetFOV();
    float NextFOV = CurrentFOV + FOVChangeSpeed * DeltaTime;
    NextFOV = FMath::Clamp(NextFOV, MinFOV_ADS, MaxFOV_ADS);
    SetFOV(NextFOV);
}

void ARabbitPlayer::TakePicture()
{
    if (IsADS() && GetRabbitCamera())
    {
        GetRabbitCamera()->TakePicture();
    }
}

void ARabbitPlayer::ToggleADS()
{
    if (bIsADS)
    {
        EndADS();
    }
    else
    {
        StartADS();
    }
}

void ARabbitPlayer::StartADS()
{
    if (bIsADS)
    {
        return;
    }
    
    bIsADS = true;
    
    CameraShakeInstance = GetPlayerController()->PlayerCameraManager->StartCameraShake(IdleCameraShake);

    if (auto CameraMeshComp = GetComponentByClass<UCameraMeshComponent>())
    {
        CameraMeshComp->bHidden = true;
    }

    SetFOV(DefaultFOV_ADS);
}

void ARabbitPlayer::EndADS()
{
    if (!bIsADS)
    {
        return;
    }
    
    bIsADS = false;
    
    GetPlayerController()->PlayerCameraManager->StopCameraShake(CameraShakeInstance, true);
    CameraShakeInstance = nullptr;

    if (auto CameraMeshComp = GetComponentByClass<UCameraMeshComponent>())
    {
        CameraMeshComp->bHidden = false;
    }

    SetFOV(DefaultFOV);
}

void ARabbitPlayer::SetFOV(float FOV)
{
    if (UCameraComponent* CameraComp = GetComponentByClass<UCameraComponent>())
    {
        CameraComp->ViewFOV = FOV;
    }
    else if (GetPlayerController())
    {
        GetPlayerController()->PlayerCameraManager->SetFOV(FOV);
    }
}

float ARabbitPlayer::GetFOV() const
{
    if (UCameraComponent* CameraComp = GetComponentByClass<UCameraComponent>())
    {
        return CameraComp->ViewFOV;
    }
    else if (GetPlayerController())
    {
        return GetPlayerController()->PlayerCameraManager->GetFOV();
    }
    return 0.f;
}
