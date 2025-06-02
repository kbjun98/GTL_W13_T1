#pragma once
#include "Components/StaticMeshComponent.h"


class UCameraMeshComponent : public UStaticMeshComponent
{
    DECLARE_CLASS(UCameraMeshComponent, UStaticMeshComponent)

public:
    UCameraMeshComponent();
    virtual ~UCameraMeshComponent() override = default;

    virtual void TickComponent(float DeltaTime) override;
};
