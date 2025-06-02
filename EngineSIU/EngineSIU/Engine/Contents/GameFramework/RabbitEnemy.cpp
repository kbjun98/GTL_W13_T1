#include "RabbitEnemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Contents/GameFramework/RabbitMovementComponent.h"

void ARabbitEnemy::PostSpawnInitialize()
{
    Super::PostSpawnInitialize();
}

UObject* ARabbitEnemy::Duplicate(UObject* InOuter)
{
    ARabbitEnemy* NewEnemy = Cast<ARabbitEnemy>(Super::Duplicate(InOuter));
    return NewEnemy;
}

void ARabbitEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ARabbitEnemy::BeginPlay()
{
    Super::BeginPlay();    
    URabbitMovementComponent* MovementComponent = GetComponentByClass<URabbitMovementComponent>();
    MovementComponent->MaxSpeed = 200.0f; // 적의 이동 속도 설정    
}

void ARabbitEnemy::RoatateToTarget(const FVector& Location, float DeltaTime)
{
    if (USkeletalMeshComponent* SkeletalMeshComponent = GetComponentByClass<USkeletalMeshComponent>())
    {
        FVector Dir = Location - GetActorLocation();
        Dir.Z = 0.0f; // Z축 방향은 무시
        if (!Dir.IsNearlyZero())
        {
            float TargetYawRad = FMath::Atan2(Dir.Y, Dir.X);
            float TargetYawDeg = FMath::RadiansToDegrees(TargetYawRad);

            FRotator CurrentRot = SkeletalMeshComponent->GetComponentRotation();
            FRotator TargetRot(0.0f, TargetYawDeg, 0.0f);

            float InterpSpeed = 10.0f;
            FRotator SmoothRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, InterpSpeed);
            SkeletalMeshComponent->SetWorldRotation(SmoothRot);
            /*Dir.GetSafeNormal();
            float YawRad = FMath::Atan2(Dir.Y, Dir.X);
            float YawDeg = FMath::RadiansToDegrees(YawRad);
            SkeletalMeshComponent->SetWorldRotation(FRotator(0.0f, YawDeg, 0.0f).Quaternion());*/
        }
    }
}
