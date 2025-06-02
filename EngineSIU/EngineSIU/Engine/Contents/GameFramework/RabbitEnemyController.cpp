#include "RabbitEnemyController.h"
#include "Engine/Contents/GameFramework/RabbitPlayer.h"
#include "Engine/Contents/GameFramework/RabbitEnemy.h"

void ARabbitEnemyController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    ProcessEnemyMovement(DeltaTime);
}

void ARabbitEnemyController::ProcessEnemyMovement(float DeltaTime)
{
    if (IsChasing)
    {
        MoveTo(GetTargetRabbitPlayer()->GetActorLocation());
    }
}

ARabbitPlayer* ARabbitEnemyController::GetTargetRabbitPlayer()
{
    return Cast<ARabbitPlayer>(TargetPawn);
}

ARabbitEnemy* ARabbitEnemyController::GetPossesedRabbitEnemy()
{
    return Cast<ARabbitEnemy>(PossessedPawn);
}

void ARabbitEnemyController::MoveTo(const FVector& TargetLocation)
{
    if (ARabbitEnemy* Enemy = GetPossesedRabbitEnemy())
    {
        FVector TargetDirection = TargetLocation - Enemy->GetActorLocation();
        Enemy->AddMovementInput(TargetDirection, 1.0f);
    }
}

