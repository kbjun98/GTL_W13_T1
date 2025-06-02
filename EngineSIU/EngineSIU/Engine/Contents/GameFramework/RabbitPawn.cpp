#include "RabbitPawn.h"

#include "Camera/CameraComponent.h"
#include "RabbitMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PlayerController.h"
#include "Math/JungleMath.h"
#include "Components/SkeletalMeshComponent.h"

void ARabbitPawn::PostSpawnInitialize()
{
    APawn::PostSpawnInitialize();


    UCapsuleComponent* Collision = AddComponent<UCapsuleComponent>("Collision_0");
    Collision->SetHalfHeight(HalfHeight);
    Collision->SetRadius(Radius);
    RootComponent = Collision;

    USkeletalMeshComponent* Mesh = AddComponent<USkeletalMeshComponent>("Mesh_0");
    Mesh->SetupAttachment(RootComponent);
    Mesh->SetRelativeLocation(FVector(0.f, 0.f, -40.0f));
    
    UCameraComponent* Camera = AddComponent<UCameraComponent>("Camera_0");
    Camera->SetupAttachment(RootComponent);
    
    MovementComponent = AddComponent<URabbitMovementComponent>("RabbitMoveComp_0");

    RabbitCam = std::make_shared<RabbitCamera>();
}

UObject* ARabbitPawn::Duplicate(UObject* InOuter)
{
    ARabbitPawn* NewPawn = Cast<ARabbitPawn>(Super::Duplicate(InOuter));
    NewPawn->RabbitCam = std::make_shared<RabbitCamera>();
    
    return NewPawn;
}

void ARabbitPawn::BeginPlay()
{
    APawn::BeginPlay();

    if (UCapsuleComponent* Collision = Cast<UCapsuleComponent>(RootComponent))
    {
        Collision->OnComponentBeginOverlap.AddLambda(
            [](UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
            {
                UE_LOG(ELogLevel::Display, TEXT("Begin Overlap!"));
            }
        );

        Collision->OnComponentEndOverlap.AddLambda(
            [](UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
            {
                UE_LOG(ELogLevel::Display, TEXT("End Overlap!"));
            }
        );
    }
}

void ARabbitPawn::Tick(float DeltaTime)
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

std::shared_ptr<RabbitCamera> ARabbitPawn::GetRabbitCamera()
{
    return RabbitCam;
}

FVector ARabbitPawn::GetActorForwardVector() const
{
    if (GetPlayerController())
    {
        const FRotator& ControlRotation = GetPlayerController()->GetControlRotation();
        FRotator ActualRotation = FRotator(0.f, ControlRotation.Yaw, 0.f);
        return ActualRotation.ToVector();
    }
    return APawn::GetActorForwardVector();
}

FVector ARabbitPawn::GetActorRightVector() const
{
    if (GetPlayerController())
    {
        const FRotator& ControlRotation = GetPlayerController()->GetControlRotation();
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

void ARabbitPawn::SetMaxHealth(int32 Value)
{
    MaxHealth = Value;
    CurrentHealth = FMath::Min(CurrentHealth, MaxHealth);
}

void ARabbitPawn::SetCurrentHealth(int32 Value)
{
    CurrentHealth = FMath::Min(Value, MaxHealth);
}
