#pragma once
#include "GameFramework/Actor.h"


class UBoxComponent;

class ASuccessVolume : public AActor
{
    DECLARE_CLASS(ASuccessVolume, AActor)

public:
    ASuccessVolume() = default;
    virtual ~ASuccessVolume() override = default;

    virtual void PostSpawnInitialize() override;

    UBoxComponent* BoxComponent;
};
