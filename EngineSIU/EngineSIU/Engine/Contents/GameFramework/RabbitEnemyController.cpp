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
    CheckStateChange();
    ProcessEnemyMovement(DeltaTime);
}

void ARabbitEnemyController::ProcessEnemyMovement(float DeltaTime)
{
    ARabbitEnemy* Enemy = GetPossesedRabbitEnemy();
    switch (CurrentState)
    {
    case EnemyState::IDLE:
        Enemy->SetAnimState(ERabbitAnimState::EIDLE);
        break;
    case EnemyState::CHASE:
        Enemy->SetAnimState(ERabbitAnimState::EWALK);
        MoveTo(GetTargetRabbitPlayer()->GetActorLocation());
        break;
    case EnemyState::ATTACK:
        Enemy->SetAnimState(ERabbitAnimState::EAttack);
        break;
    default:
        break;
    }

}

void ARabbitEnemyController::CheckStateChange()
{
    ARabbitEnemy* Enemy = GetPossesedRabbitEnemy();
    switch (CurrentState)
    {
    case EnemyState::IDLE:
        if (IsTargetInSight(Enemy->ChaseRangeMin, Enemy->ChaseRangeMax))
        {
            CurrentState = EnemyState::CHASE;
        }
        else if (IsTargetInSight(0, Enemy->AcceptanceRadius))
        {
            CurrentState = EnemyState::ATTACK;
        }
        break;
    case EnemyState::CHASE:
        if (IsTargetInSight(0, Enemy->AcceptanceRadius))
        {
            CurrentState = EnemyState::ATTACK;            
        }
        else if (IsTargetInRange(Enemy->FailureRadius, FLT_MAX))
        {
            CurrentState = EnemyState::IDLE;
        }
        break;
    case EnemyState::ATTACK:
        if (IsTargetInRange(Enemy->AcceptanceRadius, Enemy->ChaseRangeMin)
            ||!IsTargetInFOV(Enemy->SightFOV))
        {
            CurrentState = EnemyState::IDLE;
        }
        else if (IsTargetInSight(Enemy->ChaseRangeMin, Enemy->ChaseRangeMax))
        {
            CurrentState = EnemyState::CHASE;
        }
        break;
    default:
        break;
    }
}

bool ARabbitEnemyController::IsTargetInFOV(float FOV)
{
    ARabbitEnemy* Enemy = GetPossesedRabbitEnemy();
    ARabbitPawn* Target = GetTargetRabbitPlayer();
    FVector Forward = Enemy->GetComponentByClass<USkeletalMeshComponent>()->GetForwardVector();
    FVector ToTarget = Target->GetActorLocation() - Enemy->GetActorLocation();
    float Dot = FVector::DotProduct(Forward.GetSafeNormal(), ToTarget.GetSafeNormal());
    float CosHalfFOV = FMath::Cos(FMath::DegreesToRadians(FOV / 2.0f));
    return Dot > CosHalfFOV;
}

bool ARabbitEnemyController::IsTargetInSight(float Min, float Max)
{
    return IsTargetInRange(Min,Max) && IsTargetInFOV(GetPossesedRabbitEnemy()->SightFOV);
}

bool ARabbitEnemyController::IsTargetInRange(float Min, float Max)
{
    ARabbitEnemy* Enemy = GetPossesedRabbitEnemy();
    ARabbitPawn* Target = GetTargetRabbitPlayer();
    FVector Distance = Target->GetActorLocation() - Enemy->GetActorLocation();
    return Distance.Size() <= Max && Distance.Size() >= Min;
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

