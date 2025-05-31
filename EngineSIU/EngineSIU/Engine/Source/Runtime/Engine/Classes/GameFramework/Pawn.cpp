#include "Pawn.h"

void APawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APawn::PossessedBy(APlayerController* NewController)
{
    Controller = NewController;
}
