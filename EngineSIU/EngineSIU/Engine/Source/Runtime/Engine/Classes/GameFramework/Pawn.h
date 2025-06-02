#pragma once
#include "GameFramework/Actor.h"

class AController;
class UInputComponent;
class UPawnMovementComponent;
class APlayerController;

class APawn : public AActor
{
    DECLARE_CLASS(APawn, AActor)

public:
    APawn();

    virtual void Tick(float DeltaTime) override;
    virtual void PossessedBy(AController* NewController);
    
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

    APlayerController* GetPlayerController() const;

    AController* GetController() const;
    
    void AddMovementInput(const FVector& WorldDirection, float ScaleValue = 1.f);

    void AddControllerYawInput(float Value);

    void AddControllerPitchInput(float Value);

    UPawnMovementComponent* GetMovementComponent() const;

    FVector ConsumeMovementInputVector();

protected:
    FVector ControlledInputVector;

    FVector LastControlledInputVector;
    AController* Controller = nullptr; // 이 Pawn을 소유하는 PlayerController
};
