#pragma once
#include "GameFramework/GameMode.h"
class AChaserGameMode : public AGameMode
{
    DECLARE_CLASS(AChaserGameMode, AGameMode)
public:
    AChaserGameMode();
    virtual ~AChaserGameMode() override = default;
    
    void StartMatch() override;
};

