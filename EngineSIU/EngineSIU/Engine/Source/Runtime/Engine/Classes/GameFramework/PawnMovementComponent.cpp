#include "PawnMovementComponent.h"
#include "GameFramework/Pawn.h"

UObject* UPawnMovementComponent::Duplicate(UObject* InOuter)
{
    UPawnMovementComponent* NewComponent = Cast<UPawnMovementComponent>(Super::Duplicate(InOuter));
    if (NewComponent)
    {
        // 추가적인 복사 작업이 필요하다면 여기에 작성
    }
    return NewComponent;
}

void UPawnMovementComponent::TickComponent(float DeltaTime)
{
    Super::TickComponent(DeltaTime);
}
