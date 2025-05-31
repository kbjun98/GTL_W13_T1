#include "Pawn.h"
#include "Components/InputComponent.h"
#include "GameFramework/PawnMovementComponent.h"

APawn::APawn()
    : Controller(nullptr)
    , ControlledInputVector(FVector::ZeroVector)
    ,LastControlledInputVector(FVector::ZeroVector)
{}

void APawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
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

FVector APawn::ConsumeMovementInputVector()
{
    LastControlledInputVector = ControlledInputVector;
    ControlledInputVector = FVector::ZeroVector;
    return LastControlledInputVector;
}
