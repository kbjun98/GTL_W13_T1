#pragma once
#include "Actor.h"
class AAIController : public AActor
{
    DECLARE_CLASS(AAIController, AActor)
public:
    AAIController() = default;
    virtual ~AAIController() override = default;
};

