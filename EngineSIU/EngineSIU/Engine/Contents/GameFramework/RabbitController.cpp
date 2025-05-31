#include "RabbitController.h"
#include "Engine/Contents/GameFramework/RabbitPawn.h"
#include "UObject/Casts.h"
void ARabbitController::SetupInputComponent()
{

    Super::SetupInputComponent();
    InputComponent->BindAction("W", [this](float DeltaTime) { MoveForward(DeltaTime); });
    InputComponent->BindAction("S", [this](float DeltaTime) { MoveForward(-DeltaTime); });
    InputComponent->BindAction("D", [this](float DeltaTime) { MoveRight(DeltaTime); });
    InputComponent->BindAction("A", [this](float DeltaTime) { MoveRight(-DeltaTime); });
}

void ARabbitController::MoveForward(float DeltaTime)
{
    if (ARabbitPawn* RabbitPawn = Cast<ARabbitPawn>(PossessedPawn))
    {
        RabbitPawn->MoveForward(DeltaTime);
    }
}

void ARabbitController::MoveRight(float DeltaTime)
{
    if (ARabbitPawn* RabbitPawn = Cast<ARabbitPawn>(PossessedPawn))
    {
        RabbitPawn->MoveRight(DeltaTime);
    }
}

void ARabbitController::RotateYaw(float DeltaTime)
{
    if (ARabbitPawn* RabbitPawn = Cast<ARabbitPawn>(PossessedPawn))
    {
        RabbitPawn->RotateYaw(DeltaTime);
    }
}

void ARabbitController::RotatePitch(float DeltaTime)
{
    if (ARabbitPawn* RabbitPawn = Cast<ARabbitPawn>(PossessedPawn))
    {
        RabbitPawn->RotatePitch(DeltaTime);
    }
}
