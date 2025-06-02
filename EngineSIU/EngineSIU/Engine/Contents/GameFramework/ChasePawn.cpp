#include "ChasePawn.h"
#include "ChaserController.h"

void AChasePawn::BeginPlay()
{
    Super::BeginPlay();

    if (AChaserController* ChaserController = Cast<AChaserController>(Controller))
    {
        ChaserController->Possess(this);
    }
}

void AChasePawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // FIXME : 추후 MoveComponent 로직으로 대체
    FVector CurrentLocation = GetActorLocation();
    FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
    FVector NewLocation = CurrentLocation + Direction * MoveSpeed * DeltaTime;

    SetActorLocation(NewLocation);
}

void AChasePawn::SetTargetLocation(const FVector& NewTarget)
{
    TargetLocation = NewTarget;
}

bool AChasePawn::Destroy()
{
    const bool bResult = Super::Destroy();
    if (bResult)
    {
        OnDestroyed.Broadcast(this);
    }
    return bResult;
}
