#include "RabbitPawn.h"
#include "GameFramework/PawnMovementComponent.h"

void ARabbitPawn::PostSpawnInitialize()
{
    APawn::PostSpawnInitialize();

    
    
    // 무브먼트 컴포넌트는 마지막에 추가
    MovementComponent = AddComponent<UPawnMovementComponent>("PawnMoveComp_0");
}

void ARabbitPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
