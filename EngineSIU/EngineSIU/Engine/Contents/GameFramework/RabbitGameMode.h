#pragma once
#include "GameFramework/GameMode.h"
class ARabbitGameMode : public AGameMode
{
    DECLARE_CLASS(ARabbitGameMode, AGameMode)
public:
    ARabbitGameMode();
    virtual ~ARabbitGameMode() override = default;
};

