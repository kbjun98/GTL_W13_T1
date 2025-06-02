#pragma once

#include<UObject/Object.h>
#include "UObject/ObjectMacros.h"

enum class ERabbitState
{
    EIDLE,
    EWALK,
    EAttack,
};

class RabbitAnimStateMachine : public UObject
{
    DECLARE_CLASS(RabbitAnimStateMachine, UObject)

public:
    RabbitAnimStateMachine();
    virtual ~RabbitAnimStateMachine() override = default;
    void ProcessState();

    FString GetStateName(ERabbitState State) const;
    ERabbitState GetState() const;

    void SetRabbitAnimState(ERabbitState State);
private:
    ERabbitState CurrentState;
    int MoveSpeed;
    bool bIsDancing;
};

