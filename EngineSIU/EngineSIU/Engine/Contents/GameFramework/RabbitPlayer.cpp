#include "RabbitPlayer.h"

#include "RabbitController.h"
#include "GameFramework/PlayerController.h"
#include "Math/JungleMath.h"
#include "RabbitMovementComponent.h"
#include "Camera/CameraShakeBase.h"
#include "Components/CameraMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"

void ARabbitPlayer::PostSpawnInitialize()
{
    Super::PostSpawnInitialize();
    
    UCameraComponent* Camera = AddComponent<UCameraComponent>("Camera_0");
    Camera->SetupAttachment(RootComponent);

    CameraMesh = AddComponent<UCameraMeshComponent>("CameraMesh_0");
    CameraMesh->SetupAttachment(Camera);

    RabbitCam = std::make_shared<RabbitCamera>();
    RabbitCam->SetOwner(this);

    if (SkeletalMeshComp)
    {
        SkeletalMeshComp->bHidden = true;
        SkeletalMeshComp->bSimulate = false;
    }
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

bool ARabbitPlayer::SetActorLocation(const FVector& NewLocation)
{
    Super::SetActorLocation(NewLocation);

    if (MovementComponent)
    {
        MovementComponent->SetLocation(NewLocation);
    }

    return true;
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

    if (ARabbitController* RC = GetRabbitController())
    {
        float ZoomRatio = (NextFOV - MinFOV_ADS) / (MaxFOV_ADS - MinFOV_ADS);
        float NewMouseSensitivity = ZoomRatio * (RC->GetMaxMouseSensitivity() - RC->GetMinMouseSensitivity()) + RC->GetMinMouseSensitivity();
        RC->MouseSensitivityCurrent = NewMouseSensitivity;
    }
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

    if (ARabbitController* RC = GetRabbitController())
    {
        float ZoomRatio = (NextFOV - MinFOV_ADS) / (MaxFOV_ADS - MinFOV_ADS);
        float NewMouseSensitivity = ZoomRatio * (RC->GetMaxMouseSensitivity() - RC->GetMinMouseSensitivity()) + RC->GetMinMouseSensitivity();
        RC->MouseSensitivityCurrent = NewMouseSensitivity;
    }
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

    if (ARabbitController* RC = GetRabbitController())
    {
        RC->MouseSensitivityCurrent = RC->GetBaseMouseSensitivity();
    }
}

void ARabbitPlayer::ResetPlayer()
{
    bIsDied = false;

    if (SkeletalMeshComp)
    {
        USkeletalMesh* MeshAsset = Cast<USkeletalMesh>(UAssetManager::Get().GetAsset(EAssetType::SkeletalMesh, "Contents/Bunny/Bunny2"));
        SkeletalMeshComp->SetSkeletalMeshAsset(MeshAsset);
        SkeletalMeshComp->SetAnimationMode(EAnimationMode::AnimationSingleNode);
        SkeletalMeshComp->SetRelativeLocation(FVector(0.f, 0.f, 100.f));

        SkeletalMeshComp->RigidBodyType = ERigidBodyType::KINEMATIC;
        SkeletalMeshComp->bApplyGravity = true;
        SkeletalMeshComp->bSimulate = true;

        UObject* Obj = UAssetManager::Get().GetAsset(EAssetType::PhysicsAsset, "Contents/PhysicsAsset/Bunny2");
        if (UPhysicsAsset* PhysicsAsset = Cast<UPhysicsAsset>(Obj))
        {
            SkeletalMeshComp->GetSkeletalMeshAsset()->SetPhysicsAsset(PhysicsAsset);
        }

        SkeletalMeshComp->RemovePhysXGameObject();
        
        SkeletalMeshComp->bHidden = true;
    }

    if (CameraMesh)
    {
        CameraMesh->bHidden = false;
    }

    if (GetPlayerController())
    {
        GetPlayerController()->SetInputEnabled(true);
    }
    if (GetRabbitController())
    {
        GetRabbitController()->SetInputMode(EInputMode::GameOnly);
    }
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

ARabbitController* ARabbitPlayer::GetRabbitController() const
{
    if (ARabbitController* RC = Cast<ARabbitController>(GetPlayerController()))
    {
        return RC;
    }
    return nullptr;
}

void ARabbitPlayer::OnRabbitBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp)
{
    Super::OnRabbitBeginOverlap(OverlappedComp, OtherActor, OtherComp);

    OnDeath();
}

void ARabbitPlayer::OnRabbitEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp)
{
    Super::OnRabbitEndOverlap(OverlappedComp, OtherActor, OtherComp);
}

void ARabbitPlayer::OnDeath()
{
    if (bIsDied)
    {
        return;        
    }
    
    if (SkeletalMeshComp)
    {
        USkeletalMesh* MeshAsset = Cast<USkeletalMesh>(UAssetManager::Get().GetAsset(EAssetType::SkeletalMesh, "Contents/Bunny/Bunny2"));
        SkeletalMeshComp->SetSkeletalMeshAsset(MeshAsset);
        SkeletalMeshComp->SetAnimationMode(EAnimationMode::AnimationSingleNode);
        SkeletalMeshComp->SetRelativeLocation(FVector(0.f, 0.f, 100.f));

        SkeletalMeshComp->RigidBodyType = ERigidBodyType::KINEMATIC;
        SkeletalMeshComp->bApplyGravity = true;
        SkeletalMeshComp->bSimulate = true;

        UObject* Obj = UAssetManager::Get().GetAsset(EAssetType::PhysicsAsset, "Contents/PhysicsAsset/Bunny2");
        if (UPhysicsAsset* PhysicsAsset = Cast<UPhysicsAsset>(Obj))
        {
            SkeletalMeshComp->GetSkeletalMeshAsset()->SetPhysicsAsset(PhysicsAsset);
        }

        SkeletalMeshComp->CreatePhysXGameObject();
        
        SkeletalMeshComp->EnableRagdoll(true);
        SkeletalMeshComp->bHidden = false;
    }

    if (CameraMesh)
    {
        CameraMesh->bHidden = true;
    }

    OnPlayerDied.ExecuteIfBound();
    bIsDied = true;

    if (GetPlayerController())
    {
        GetPlayerController()->SetInputEnabled(false);
    }
    if (GetRabbitController())
    {
        GetRabbitController()->SetInputMode(EInputMode::UIOnly);
    }
}
