#include "ChaserRabbitSpawner.h"
#include "Engine/Contents/GameFramework/ChasePawn.h"
#include "Engine/World/World.h"
#include <Engine/Contents/GameFramework/ChaserController.h>
AChaserRabbitSpawner::AChaserRabbitSpawner()
{
    InitialLocations.Add(FVector(0, 0, 0));
}
void AChaserRabbitSpawner::BeginPlay()
{
    Super::BeginPlay();
    InitializeChasers();
}
void AChaserRabbitSpawner::Tick(float DeltaTime)
{
}
void AChaserRabbitSpawner::InitializeChasers()
{
    for (const FVector& Location : SpawnLocations)
    {
        for (int i = 0; i < InitialLocations.Num(); i++)
        {
            AChasePawn* NewChaser = GetWorld()->SpawnActor<AChasePawn>();
            NewChaser->SetActorLocation(InitialLocations[i]);
            NewChaser->OnDestroyed.AddUObject(this, &AChaserRabbitSpawner::OnChaserRabbitDestroyed);
            SpawnedMonsters.Add(NewChaser);

            AChaserController* NewController = GetWorld()->SpawnActor<AChaserController>();
            NewController->Possess(NewChaser);
        }
    }
}

void AChaserRabbitSpawner::RespawnMonsters()
{
    InitializeChasers();
}

void AChaserRabbitSpawner::OnChaserRabbitDestroyed(AChasePawn* DestroyedPawn)
{
    SpawnedMonsters.Remove(DestroyedPawn);

    if (SpawnedMonsters.Num() == 0)
    {
        RespawnMonsters();
    }
}
