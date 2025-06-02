#pragma once
#include "Components/StaticMeshComponent.h"


class UCameraMeshComponent : public UStaticMeshComponent
{
    DECLARE_CLASS(UCameraMeshComponent, UStaticMeshComponent)

public:
    UCameraMeshComponent();
    virtual ~UCameraMeshComponent() override = default;

    virtual void TickComponent(float DeltaTime) override;

private:
    FVector TargetLocalLocation;
    FRotator TargetLocalRotation;

    FVector AnimLocalLocation;

    float InterpSpeed = 1.f;

    bool bIsMoving = false;

    float MovingTime = 0.f;

    void Anim(float DeltaTime);
};
