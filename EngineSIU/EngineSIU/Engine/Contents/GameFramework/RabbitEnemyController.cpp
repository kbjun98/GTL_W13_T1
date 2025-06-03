#include "RabbitEnemyController.h"
#include "Engine/Contents/GameFramework/RabbitPlayer.h"
#include "Engine/Contents/GameFramework/RabbitEnemy.h"
#include "Engine/Engine.h"
#include "World/World.h"
#include "Engine/Contents/Navigation/PathFinder.h"
#include <Engine/EditorEngine.h>

void ARabbitEnemyController::PostSpawnInitialize()
{
    Super::PostSpawnInitialize();
    TargetPawn = GEngine->ActiveWorld->GetMainPlayer();    
}

void ARabbitEnemyController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (CurrentState == EEnemyState::ATTACK)
    {
        float NextAttackElapsedTime = AttackElapsedTime + DeltaTime;
        AttackElapsedTime = FMath::Fmod(NextAttackElapsedTime, AnimTime);
        if (AttackElapsedTime < NextAttackElapsedTime)
        {
            bAttacked = false;
        }
    }
    
    CheckStateChange(DeltaTime);
    ProcessEnemyMovement(DeltaTime);
}

void ARabbitEnemyController::ProcessEnemyMovement(float DeltaTime)
{
    ARabbitEnemy* Enemy = GetPossessedRabbitEnemy();
    switch (CurrentState)
    {
    case EEnemyState::IDLE:
        Enemy->SetAnimState(ERabbitAnimState::Idle);
        break;
    case EEnemyState::PATROL:
        Enemy->SetAnimState(ERabbitAnimState::Walk);
        // Patrol 경로 기반 이동 등 로직을 작성
        // 경로 따라 이동
        if (!CurrentPath.IsEmpty() && CurrentPathIndex < CurrentPath.Num())
        {
            FVector NextPathPoint = CurrentPath[CurrentPathIndex];
            FVector ToNext = NextPathPoint - Enemy->GetActorLocation();
            ToNext.Z = 0.0f;

            if (ToNext.Length2DSquared() < Enemy->AcceptanceRadius * Enemy->AcceptanceRadius)
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
            Enemy->RotateToTarget(NextPathPoint, DeltaTime);
        }
        else
        {
            // 경로가 없으면 새로 계산
            UpdatePatrolPath();
        }
        break;
    case EEnemyState::CHASE:
        if (ARabbitPawn* Rabbit = Cast<ARabbitPawn>(GetPossessedPawn()))
        {
            Rabbit->Jump();
        }
        
        Enemy->SetAnimState(ERabbitAnimState::Chase);

        // 경로 기반으로 이동
        if (!CurrentPath.IsEmpty() && CurrentPathIndex < CurrentPath.Num())
        {
            FVector NextPathPoint = CurrentPath[CurrentPathIndex];
            FVector CurrentLocation = Enemy->GetActorLocation();

            // 방향 계산
            FVector TargetDirection = NextPathPoint - CurrentLocation;
            TargetDirection.Z = 0.0f;

            // 거의 도착했다면 다음 경로로 넘어감
            if (TargetDirection.Length2DSquared() < Enemy->AcceptanceRadius * Enemy->AcceptanceRadius)
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
            Enemy->RotateToTarget(NextPathPoint, DeltaTime);
        }
        else
        {
            // 경로가 비었으면 새로 계산
            UpdatePath();
        }
        break;
    case EEnemyState::ATTACK:
    {
        Enemy->SetAnimState(ERabbitAnimState::Attack);
        
        if (!bAttacked && AttackElapsedTime >= AttackTime)
        {
            bAttacked = true;
            
            const APawn* MainPlayer = GEngine->ActiveWorld->GetMainPlayer();
            ARabbitPlayer* RabbitPlayer = Cast<ARabbitPlayer>(MainPlayer);
        
            const FVector TargetLocation = RabbitPlayer->GetActorLocation();
            const FVector EnemyLocation = Enemy->GetActorLocation();
            const FVector Delta = TargetLocation - EnemyLocation;

            const float DeltaLengthSquare = Delta.Length2DSquared();
            const float AttackRangeSquare = AttackRange * AttackRange;
            if (DeltaLengthSquare < AttackRangeSquare)
            {
                RabbitPlayer->OnAttacked();
            }
        }
        break;
    }
    default:
        break;
    }

}

void ARabbitEnemyController::CheckStateChange(float DeltaTime)
{
    ARabbitEnemy* Enemy = GetPossessedRabbitEnemy();
    switch (CurrentState)
    {
    case EEnemyState::IDLE:
        IdleTime += DeltaTime;
        if (IsTargetInSight(Enemy->ChaseRangeMin, Enemy->ChaseRangeMax))
        {
            SetState(EEnemyState::CHASE);
        }
        else if (IsTargetInSight(0, Enemy->AcceptanceRadius))
        {
            SetState(EEnemyState::ATTACK);
        }
        else if (IdleTime >= 2.0f) // 2초 이상 경과했으면
        {
            SetState(EEnemyState::PATROL);
            IdleTime = 0.0f; // 초기화
        }
        break;
    case EEnemyState::PATROL:
        if (IsTargetInSight(Enemy->ChaseRangeMin, Enemy->ChaseRangeMax))
        {
            SetState(EEnemyState::CHASE);
        }
        else if (IsTargetInSight(0, Enemy->AcceptanceRadius))
        {
            SetState(EEnemyState::ATTACK);
        }
        break;
    case EEnemyState::CHASE:
        if (IsTargetInSight(0, Enemy->AcceptanceRadius))
        {
            SetState(EEnemyState::ATTACK);            
        }
        else if (IsTargetInRange(Enemy->FailureRadius, FLT_MAX))
        {
            SetState(EEnemyState::IDLE);
        }
        break;
    case EEnemyState::ATTACK:
        if (IsTargetInRange(Enemy->AcceptanceRadius, Enemy->ChaseRangeMin) ||!IsTargetInFOV(Enemy->SightFOV))
        {
            SetState(EEnemyState::IDLE);
        }
        else if (IsTargetInSight(Enemy->ChaseRangeMin, Enemy->ChaseRangeMax))
        {
            SetState(EEnemyState::CHASE);
        }
        break;
    default:
        break;
    }
}

bool ARabbitEnemyController::IsTargetInFOV(float FOV)
{
    ARabbitPlayer* Target = GetTargetRabbitPlayer();
    if (Target->IsDead())
    {
        return false;
    }
    
    ARabbitEnemy* Enemy = GetPossessedRabbitEnemy();
    FVector Forward = Enemy->GetComponentByClass<USkeletalMeshComponent>()->GetForwardVector();
    FVector ToTarget = Target->GetActorLocation() - Enemy->GetActorLocation();
    float Dot = FVector::DotProduct(Forward.GetSafeNormal(), ToTarget.GetSafeNormal());
    float CosHalfFOV = FMath::Cos(FMath::DegreesToRadians(FOV / 2.0f));
    return Dot > CosHalfFOV;
}

bool ARabbitEnemyController::IsTargetInSight(float Min, float Max)
{
    return IsTargetInRange(Min,Max) && IsTargetInFOV(GetPossessedRabbitEnemy()->SightFOV);
}

bool ARabbitEnemyController::IsTargetInRange(float Min, float Max)
{
    ARabbitPlayer* Target = GetTargetRabbitPlayer();
    if (Target->IsDead())
    {
        return false;
    }
    
    ARabbitEnemy* Enemy = GetPossessedRabbitEnemy();
    FVector Distance = Target->GetActorLocation() - Enemy->GetActorLocation();
    return Distance.Length2D() <= Max && Distance.Length2D() >= Min;
}

ARabbitPlayer* ARabbitEnemyController::GetTargetRabbitPlayer()
{
    return Cast<ARabbitPlayer>(TargetPawn);
}

ARabbitEnemy* ARabbitEnemyController::GetPossessedRabbitEnemy()
{
    return Cast<ARabbitEnemy>(PossessedPawn);
}

void ARabbitEnemyController::UpdatePatrolPath()
{
    FGridMap* GridMap = GEngine->ActiveWorld->GetGridMap();
    
    ARabbitEnemy* Enemy = GetPossessedRabbitEnemy();
    
    if (!Enemy || Enemy->PatrolTargets.Num() == 0)
    {
        return;
    }

    USceneComponent* TargetComp = Enemy->PatrolTargets[CurPatrolIndex];
    FVector TargetLocation = TargetComp->GetComponentLocation();

    FVector StartLocation = Enemy->GetActorLocation();

    // 경로 계산
    CurrentPath = PathFinder->FindWorlPosPathByWorldPos(*GridMap, StartLocation, TargetLocation);
    CurrentPathIndex = 0;

    CurPatrolIndex = (CurPatrolIndex + 1) % Enemy->PatrolTargets.Num();
}

void ARabbitEnemyController::MoveTo(const FVector& TargetLocation, float DeltaTime)
{
    if (ARabbitEnemy* Enemy = GetPossessedRabbitEnemy())
    {
        FVector TargetDirection = TargetLocation - Enemy->GetActorLocation();    
        TargetDirection.Z = 0.0f; // Z축 방향은 무시
        FVector DirNormal = TargetDirection.GetSafeNormal();
        Enemy->AddMovementInput(DirNormal, 100.0f);
        Enemy->RotateToTarget(TargetLocation, DeltaTime);
    }
}

void ARabbitEnemyController::SetState(EEnemyState NewState)
{
    CurrentState = NewState;

    if (CurrentState == EEnemyState::ATTACK)
    {
        AttackElapsedTime = 0.f;
    }
}

