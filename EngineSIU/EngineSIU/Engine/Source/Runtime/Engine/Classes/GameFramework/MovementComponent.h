#pragma once
#include "Components/ActorComponent.h"
#include "Engine/HitResult.h"

class USceneComponent;

class UMovementComponent : public UActorComponent
{
    DECLARE_CLASS(UMovementComponent, UActorComponent)
    
public:
    UMovementComponent();

    virtual void InitializeComponent() override;
    
    virtual UObject* Duplicate(UObject* InOuter) override;

    virtual void BeginPlay() override;

    virtual void TickComponent(float DeltaTime) override;

    virtual void SetUpdatedComponent(USceneComponent* NewUpdatedComponent);

    USceneComponent* UpdatedComponent;
    
    FVector Velocity; // 현재 속도

    float MaxSpeed;

    bool MoveUpdatedComponent(const FVector& Delta, const FQuat& NewRotation);
    bool MoveUpdatedComponent(const FVector& Delta, const FRotator& NewRotation);

protected:
    virtual bool MoveUpdatedComponentImpl(const FVector& Delta, const FQuat& NewRotation);
};

