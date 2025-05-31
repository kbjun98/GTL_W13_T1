#pragma once
#include "GameFramework/Actor.h"

class APlayerController;

class APawn : public AActor
{
    DECLARE_CLASS(APawn, AActor)

public:
    APawn() = default;

    virtual void Tick(float DeltaTime) override;
    virtual void PossessedBy(APlayerController* NewController);

    APlayerController* Controller = nullptr; // 이 Pawn을 소유하는 PlayerController
};
