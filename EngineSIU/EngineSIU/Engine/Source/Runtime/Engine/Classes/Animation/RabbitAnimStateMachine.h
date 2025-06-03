#pragma once

#include<UObject/Object.h>
#include "UObject/ObjectMacros.h"

enum class ERabbitAnimState
{
    Idle,
    Walk,
    Chase,
    Attack,
};

class RabbitAnimStateMachine : public UObject
{
    DECLARE_CLASS(RabbitAnimStateMachine, UObject)

public:
    RabbitAnimStateMachine()=default;
    virtual ~RabbitAnimStateMachine() override = default;
    void ProcessState(ERabbitAnimState State);

    FString GetStateName(ERabbitAnimState State) const;

private:
};

