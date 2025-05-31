#include "Pawn.h"
#include "Components/InputComponent.h"
#include "GameFramework/PawnMovementComponent.h"

APawn::APawn()
    : Controller(nullptr)
    , ControlledInputVector(FVector::ZeroVector)
{}

void APawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (UPawnMovementComponent* MoveComp = GetMovementComponent())
    {
        FVector Delta = ControlledInputVector * MoveComp->MaxSpeed * DeltaTime;
        MoveComp->MoveUpdatedComponent(Delta, GetActorRotation());
    }

    ControlledInputVector = FVector::ZeroVector;
}

void APawn::PossessedBy(APlayerController* NewController)
{
    Controller = NewController;
}

void APawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
}

void APawn::AddMovementInput(const FVector& WorldDirection, float ScaleValue)
{
    ControlledInputVector += WorldDirection * ScaleValue;
}

UPawnMovementComponent* APawn::GetMovementComponent() const
{
    return GetComponentByClass<UPawnMovementComponent>();
}
