#include "RabbitController.h"
#include "Engine/Contents/Character/RabbitPawn.h"
#include "UObject/Casts.h"
void ARabbitController::SetupInputComponent()
{

    Super::SetupInputComponent();
    if (ARabbitPawn* RabbitPawn = Cast<ARabbitPawn>(PossessedPawn))
    {
        InputComponent->BindAction("W", [RabbitPawn](float DeltaTime) { RabbitPawn->MoveForward(DeltaTime); });
        InputComponent->BindAction("S", [RabbitPawn](float DeltaTime) { RabbitPawn->MoveForward(-DeltaTime); });
        InputComponent->BindAction("D", [RabbitPawn](float DeltaTime) { RabbitPawn->MoveRight(DeltaTime); });
        InputComponent->BindAction("A", [RabbitPawn](float DeltaTime) { RabbitPawn->MoveRight(-DeltaTime); });
    }
}
