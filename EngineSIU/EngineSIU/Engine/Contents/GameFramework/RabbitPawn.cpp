#include "RabbitPawn.h"

#include "Camera/CameraComponent.h"
#include "RabbitMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PlayerController.h"
#include "Math/JungleMath.h"
#include "Components/SkeletalMeshComponent.h"
#include <AnimInstance/RabbitAnimInstance.h>

void ARabbitPawn::PostSpawnInitialize()
{
    APawn::PostSpawnInitialize();
        
    UCapsuleComponent* Collision = AddComponent<UCapsuleComponent>("Collision_0");
    Collision->SetHalfHeight(HalfHeight);
    Collision->SetRadius(Radius);
    RootComponent = Collision;
    
    SkeletalMeshComp = AddComponent<USkeletalMeshComponent>("Mesh_0");
    SkeletalMeshComp->SetupAttachment(RootComponent);
    SkeletalMeshComp->SetRelativeLocation(FVector(0.f, 0.f, -40.0f));
    SkeletalMeshComp->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    SkeletalMeshComp->AnimClass = RabbitAnimInstance::StaticClass();

    MovementComponent = AddComponent<URabbitMovementComponent>("RabbitMoveComp_0");

}

UObject* ARabbitPawn::Duplicate(UObject* InOuter)
{
    ARabbitPawn* NewPawn = Cast<ARabbitPawn>(Super::Duplicate(InOuter));
    
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
}


FVector ARabbitPawn::GetActorForwardVector() const
{
    return Super::GetActorForwardVector();
}

FVector ARabbitPawn::GetActorRightVector() const
{
    return Super::GetActorRightVector();
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
