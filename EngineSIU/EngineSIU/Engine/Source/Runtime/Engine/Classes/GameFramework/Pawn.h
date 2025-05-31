#pragma once
#include "GameFramework/Actor.h"

class APlayerController;
class UInputComponent;
class UPawnMovementComponent;

class APawn : public AActor
{
    DECLARE_CLASS(APawn, AActor)

public:
    APawn();

    virtual void Tick(float DeltaTime) override;
    
    virtual void PossessedBy(APlayerController* NewController);
    
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

    APlayerController* Controller;

    void AddMovementInput(const FVector& WorldDirection, float ScaleValue = 1.f);

    UPawnMovementComponent* GetMovementComponent() const;

    FVector ConsumeMovementInputVector();

protected:
    FVector ControlledInputVector;

    FVector LastControlledInputVector;
};
