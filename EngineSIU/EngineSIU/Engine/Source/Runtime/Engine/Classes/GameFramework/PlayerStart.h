#pragma once
#include "GameFramework/Actor.h"


class APlayerStart : public AActor
{
    DECLARE_CLASS(APlayerStart, AActor)

public:
    APlayerStart() = default;
    virtual ~APlayerStart() override = default;

    virtual void PostSpawnInitialize() override;

    virtual UObject* Duplicate(UObject* InOuter) override;
};
