#include "Pawn.h"
#include "Components/InputComponent.h"
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
