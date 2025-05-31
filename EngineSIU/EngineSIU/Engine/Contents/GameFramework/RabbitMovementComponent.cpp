#include "RabbitMovementComponent.h"

#include "GameFramework/Pawn.h"
#include "UObject/Casts.h"

URabbitMovementComponent::URabbitMovementComponent()
    : bGravity(true)
    , GravityFactor(9.8f)
{}


void URabbitMovementComponent::TickComponent(float DeltaTime)
{
    Super::TickComponent(DeltaTime);

    PerformMovement(DeltaTime);
}

void URabbitMovementComponent::PerformMovement(float DeltaTime)
{
    if (UpdatedComponent == nullptr)
    {
        return;
    }

    const FVector PrevVelocity = Velocity;
    Velocity.X = 0.f;
    Velocity.Y = 0.f;

    if (APawn* Pawn = Cast<APawn>(UpdatedComponent->GetOwner()))
    {
        FVector MoveInput = Pawn->ConsumeMovementInputVector();
        Velocity += MoveInput * MaxSpeed;
    }

    if (bGravity)
    {
        //Velocity += FVector(0.0f, 0.0f, -GravityFactor);
    }

    MoveUpdatedComponent(Velocity * DeltaTime, GetOwner()->GetActorRotation());
}
