#include "Pawn.h"

#include "PlayerController.h"
#include "Components/InputComponent.h"
#include "GameFramework/PawnMovementComponent.h"

APawn::APawn()
    : Controller(nullptr)
    , ControlledInputVector(FVector::ZeroVector)
    , LastControlledInputVector(FVector::ZeroVector)
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

APlayerController* APawn::GetController()
{
    if (!Controller)
    {
        
    }
    return Controller;
}

void APawn::AddMovementInput(const FVector& WorldDirection, float ScaleValue)
{
    ControlledInputVector += WorldDirection * ScaleValue;
}

void APawn::AddControllerYawInput(float Value)
{
    if (Controller)
    {
        Controller->AddYawInput(Value);
    }
}

void APawn::AddControllerPitchInput(float Value)
{
    if (Controller)
    {
        Controller->AddPitchInput(Value);
    }
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
