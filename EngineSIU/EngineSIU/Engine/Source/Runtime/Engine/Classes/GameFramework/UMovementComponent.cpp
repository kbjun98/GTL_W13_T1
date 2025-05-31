#include "UMovementComponent.h"
#include "GameFramework/Actor.h"
UObject* UMovementComponent::Duplicate(UObject* InOuter)
{
    ThisClass* NewComponent = Cast<ThisClass>(Super::Duplicate(InOuter));

    NewComponent->Velocity = Velocity;

    return NewComponent;
}
void UMovementComponent::TickComponent(float DeltaTime)
{
    Super::TickComponent(DeltaTime);
    FVector Location = GetOwner()->GetActorLocation();
    FVector NewLocation = Location + (Velocity * DeltaTime);
    GetOwner()->SetActorLocation(NewLocation);
}
