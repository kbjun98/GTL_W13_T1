#pragma once
#include "GameFramework/Actor.h"


class UBoxComponent;

class ADeathVolume : public AActor
{
    DECLARE_CLASS(ADeathVolume, AActor)

public:
    ADeathVolume() = default;
    virtual ~ADeathVolume() override = default;

    virtual void PostSpawnInitialize() override;

    UBoxComponent* BoxComponent;
};
