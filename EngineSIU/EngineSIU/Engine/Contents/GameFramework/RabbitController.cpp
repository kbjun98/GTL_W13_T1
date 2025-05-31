#include "RabbitController.h"
#include "Engine/Contents/GameFramework/RabbitPawn.h"
#include "UObject/Casts.h"
void ARabbitController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("W", [this](float DeltaTime) { MoveForward(); });
    InputComponent->BindAction("S", [this](float DeltaTime) { MoveBack(); });
    InputComponent->BindAction("D", [this](float DeltaTime) { MoveRight(); });
    InputComponent->BindAction("A", [this](float DeltaTime) { MoveLeft(); });
}

void ARabbitController::MoveForward()
{
    if (APawn* Pawn = GetPawn())
    {
        Pawn->AddMovementInput(GetActorForwardVector(), 1.f);
    }
}

void ARabbitController::MoveBack()
{
    if (APawn* Pawn = GetPawn())
    {
        Pawn->AddMovementInput(GetActorForwardVector(), -1.f);
    }
}

void ARabbitController::MoveRight()
{
    if (APawn* Pawn = GetPawn())
    {
        Pawn->AddMovementInput(GetActorRightVector(), 1.f);
    }
}

void ARabbitController::MoveLeft()
{
    if (APawn* Pawn = GetPawn())
    {
        Pawn->AddMovementInput(GetActorRightVector(), -1.f);
    }
}
