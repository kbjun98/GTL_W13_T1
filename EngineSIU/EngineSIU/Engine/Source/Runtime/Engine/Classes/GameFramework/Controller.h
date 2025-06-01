#pragma once
#include "Actor.h"

class APawn;

class AController : public AActor
{
    DECLARE_CLASS(AController, AActor)

public:
    AController() = default;
    virtual ~AController() override = default;

    virtual void Possess(APawn* InPawn);
    virtual void UnPossess();

    APawn* GetPossessedPawn() const { return PossessedPawn; }

protected:
    APawn* PossessedPawn = nullptr;
    bool bHasPossessed = false;
};
