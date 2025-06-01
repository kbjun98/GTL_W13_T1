#include "ChaserGameMode.h"
#include "ChasePawn.h"
#include "World/World.h"
#include "ChaserController.h"

AChaserGameMode::AChaserGameMode()
{
    DefaultPawnClass = AChasePawn::StaticClass();
    PlayerControllerClass = AChaserController::StaticClass();
}

void AChaserGameMode::StartMatch()
{
    Super::StartMatch();

    // ChaserPawn 생성
    AChasePawn* ChasePawn = GetWorld()->SpawnActor<AChasePawn>();
    // ChaserController 생성
    AChaserController* ChaserController = GetWorld()->SpawnActor<AChaserController>();

    // Possess
    ChaserController->Possess(ChasePawn);
}
