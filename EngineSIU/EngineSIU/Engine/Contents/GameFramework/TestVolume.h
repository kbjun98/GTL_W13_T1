#pragma once
#include "GameFramework/Actor.h"


class UBoxComponent;

class ATestVolume : public AActor
{
    DECLARE_CLASS(ATestVolume, AActor)

public:
    ATestVolume() = default;
    virtual ~ATestVolume() override = default;

    virtual void PostSpawnInitialize() override;

    UBoxComponent* BoxComponent;
};
