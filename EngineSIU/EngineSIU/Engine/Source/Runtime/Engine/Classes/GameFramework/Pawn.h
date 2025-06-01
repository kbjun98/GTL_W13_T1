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

    APlayerController* GetController();
    
    void AddMovementInput(const FVector& WorldDirection, float ScaleValue = 1.f);

    void AddControllerYawInput(float Value);

    void AddControllerPitchInput(float Value);

    UPawnMovementComponent* GetMovementComponent() const;

    FVector ConsumeMovementInputVector();

protected:
    FVector ControlledInputVector;

    FVector LastControlledInputVector;
};
