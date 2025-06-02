#include "RabbitEnemyController.h"
#include "Engine/Contents/GameFramework/RabbitPlayer.h"
#include "Engine/Contents/GameFramework/RabbitEnemy.h"
#include "Engine/Engine.h"
#include "World/World.h"

void ARabbitEnemyController::PostSpawnInitialize()
{
    Super::PostSpawnInitialize();
    TargetPawn = GEngine->ActiveWorld->GetMainPlayer();
}

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
        TargetDirection.Z = 0.0f; // Z축 방향은 무시
        FVector DirNormal = TargetDirection.GetSafeNormal();
        Enemy->AddMovementInput(DirNormal, 1.0f);
        Enemy->RoatateToTarget(TargetLocation);
    }
}

