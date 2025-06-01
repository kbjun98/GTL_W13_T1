#pragma once
#include "GameFramework/Actor.h"

class AController;
class UInputComponent;

class APawn : public AActor
{
    DECLARE_CLASS(APawn, AActor)

public:
    APawn() = default;

    virtual void Tick(float DeltaTime) override;
    virtual void PossessedBy(AController* NewController);
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

    AController* Controller = nullptr; // 이 Pawn을 소유하는 PlayerController
};
