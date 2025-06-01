#pragma once
#include "Classes/GameFrameWork/Actor.h"
#include "CoreUObject/UObject/ObjectMacros.h"
class AChasePawn;
class AChaserRabbitSpawner : public AActor
{
    DECLARE_CLASS(AChaserRabbitSpawner, AActor)

public:
    AChaserRabbitSpawner();

    TArray<AChasePawn*> SpawnedMonsters;
    TArray<FVector> SpawnLocations;
    TArray<FVector> InitialLocations;

public:
    
    virtual void BeginPlay() override;

    /** 매 Tick마다 호출됩니다. */
    virtual void Tick(float DeltaTime);

    void InitializeChasers();
    void RespawnMonsters();
    void OnChaserRabbitDestroyed(AChasePawn* DestroyedPawn);
};
