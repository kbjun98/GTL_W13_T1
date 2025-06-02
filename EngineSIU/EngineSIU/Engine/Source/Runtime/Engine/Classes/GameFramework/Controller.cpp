#include "Controller.h"
#include "GameFramework/Pawn.h"

void AController::Possess(APawn* InPawn)
{
    PossessedPawn = InPawn;
    bHasPossessed = true;
    InPawn->PossessedBy(this);
}

void AController::UnPossess()
{
    if (!bHasPossessed && PossessedPawn == nullptr)
    {
        UE_LOG(ELogLevel::Warning, "Invalid Pawn");
        return;
    }

    PossessedPawn = nullptr;
    bHasPossessed = false;
}
