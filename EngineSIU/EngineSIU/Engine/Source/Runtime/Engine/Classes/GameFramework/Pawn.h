#pragma once
#include "GameFramework/Actor.h"

class APlayerController;
class UInputComponent;

class APawn : public AActor
{
    DECLARE_CLASS(APawn, AActor)

public:
    APawn() = default;

    virtual void Tick(float DeltaTime) override;
    virtual void PossessedBy(APlayerController* NewController);
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

    APlayerController* Controller = nullptr; // 이 Pawn을 소유하는 PlayerController
private:
    void MoveForward(float DeltaTime);
    void MoveRight(float DeltaTime);
private:
    float MoveSpeed = 100.0f; // 이동 속도  
};
