#include "RabbitEnemy.h"
#include "Components/SkeletalMeshComponent.h"

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

void ARabbitEnemy::RoatateToTarget(const FVector& Location)
{
    if (USkeletalMeshComponent* SkeletalMeshComponent = GetComponentByClass<USkeletalMeshComponent>())
    {
        FVector Dir = Location - GetActorLocation();
        Dir.Z = 0.0f; // Z축 방향은 무시
        if (!Dir.IsNearlyZero())
        {
            Dir.GetSafeNormal();
            float YawRad = FMath::Atan2(Dir.Y, Dir.X);
            float YawDeg = FMath::RadiansToDegrees(YawRad);
            SkeletalMeshComponent->SetWorldRotation(FRotator(0.0f, YawDeg, 0.0f).Quaternion());
        }
    }
}
