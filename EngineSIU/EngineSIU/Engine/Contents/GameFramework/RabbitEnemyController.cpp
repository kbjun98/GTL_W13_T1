#include "RabbitEnemyController.h"
#include "Engine/Contents/GameFramework/RabbitPlayer.h"
#include "Engine/Contents/GameFramework/RabbitEnemy.h"
#include "Engine/Engine.h"
#include "World/World.h"
#include "Engine/Contents/Navigation/PathFinder.h"
void ARabbitEnemyController::PostSpawnInitialize()
{
    Super::PostSpawnInitialize();
    TargetPawn = GEngine->ActiveWorld->GetMainPlayer();    
}

void ARabbitEnemyController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    CheckStateChange(DeltaTime);
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
    case EnemyState::PATROL:
        Enemy->SetAnimState(ERabbitAnimState::EWALK);
        // Patrol 경로 기반 이동 등 로직을 작성
        // 경로 따라 이동
        if (!CurrentPath.IsEmpty() && CurrentPathIndex < CurrentPath.Num())
        {
            FVector NextPathPoint = CurrentPath[CurrentPathIndex];
            FVector ToNext = NextPathPoint - Enemy->GetActorLocation();
            ToNext.Z = 0.0f;

            if (ToNext.Size() < Enemy->AcceptanceRadius)
            {
                CurrentPathIndex++;
                if (CurrentPathIndex >= CurrentPath.Num())
                {
                    // PatrolTarget에 도착하면 UpdatePatrolPath() 호출
                    UpdatePatrolPath();
                    return;
                }
            }

            FVector DirNormal = ToNext.GetSafeNormal();
            Enemy->AddMovementInput(DirNormal, 1.0f);
            Enemy->RoatateToTarget(NextPathPoint, DeltaTime);
        }
        else
        {
            // 경로가 없으면 새로 계산
            UpdatePatrolPath();
        }
        break;


        break;
    case EnemyState::CHASE:
        /*Enemy->SetAnimState(ERabbitAnimState::EWALK);
        UpdatePath();
        MoveTo(CurrentPath[CurrentPathIndex]);
        break;*/

        Enemy->SetAnimState(ERabbitAnimState::EWALK);

        // 경로 기반으로 이동
        if (!CurrentPath.IsEmpty() && CurrentPathIndex < CurrentPath.Num())
        {
            FVector NextPathPoint = CurrentPath[CurrentPathIndex];
            FVector CurrentLocation = Enemy->GetActorLocation();

            // 방향 계산
            FVector TargetDirection = NextPathPoint - CurrentLocation;
            TargetDirection.Z = 0.0f;

            // 거의 도착했다면 다음 경로로 넘어감
            if (TargetDirection.Size() < Enemy->AcceptanceRadius)
            {
                CurrentPathIndex++;
                if (CurrentPathIndex >= CurrentPath.Num())
                {
                    // 경로가 끝났으면 멈춤
                    CurrentPathIndex = 0;
                    CurrentPath.Empty();
                }
                return;
            }

            FVector DirNormal = TargetDirection.GetSafeNormal();

            // 이동 방향을 MovementInput으로 전달
            Enemy->AddMovementInput(DirNormal, 1.0f);

            // 방향 회전 처리
            Enemy->RoatateToTarget(NextPathPoint, DeltaTime);
        }
        else
        {
            // 경로가 비었으면 새로 계산
            UpdatePath();
        }
        break;
    case EnemyState::ATTACK:
        Enemy->SetAnimState(ERabbitAnimState::EAttack);
        break;
    default:
        break;
    }

}

void ARabbitEnemyController::CheckStateChange(float DeltaTime)
{
    ARabbitEnemy* Enemy = GetPossesedRabbitEnemy();
    switch (CurrentState)
    {
    case EnemyState::IDLE:
        IdleTime += DeltaTime;
        if (IsTargetInSight(Enemy->ChaseRangeMin, Enemy->ChaseRangeMax))
        {
            CurrentState = EnemyState::CHASE;
        }
        else if (IsTargetInSight(0, Enemy->AcceptanceRadius))
        {
            CurrentState = EnemyState::ATTACK;
        }
        else if (IdleTime >= 2.0f) // 2초 이상 경과했으면
        {
            CurrentState = EnemyState::PATROL;
            IdleTime = 0.0f; // 초기화
        }
        break;
    case EnemyState::PATROL:
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

void ARabbitEnemyController::UpdatePatrolPath()
{
    ARabbitEnemy* Enemy = GetPossesedRabbitEnemy();
    if (!Enemy || Enemy->PatrolTargets.Num() == 0) return;

    USceneComponent* TargetComp = Enemy->PatrolTargets[CurPatrolIndex];
    FVector TargetLocation = TargetComp->GetComponentLocation();

    FVector StartLocation = Enemy->GetActorLocation();

    // 경로 계산
    CurrentPath = PathFinder->FindWorlPosPathByWorldPos(
        *GridMap, StartLocation, TargetLocation);
    CurrentPathIndex = 0;

    CurPatrolIndex = (CurPatrolIndex + 1) % Enemy->PatrolTargets.Num();
}

void ARabbitEnemyController::MoveTo(const FVector& TargetLocation, float DeltaTime)
{
    if (ARabbitEnemy* Enemy = GetPossesedRabbitEnemy())
    {
        FVector TargetDirection = TargetLocation - Enemy->GetActorLocation();    
        TargetDirection.Z = 0.0f; // Z축 방향은 무시
        FVector DirNormal = TargetDirection.GetSafeNormal();
        Enemy->AddMovementInput(DirNormal, 100.0f);
        Enemy->RoatateToTarget(TargetLocation, DeltaTime);
    }
}

