#pragma once
#include "GameFramework/GameMode.h"

class ARabbitGameMode : public AGameMode
{
    DECLARE_CLASS(ARabbitGameMode, AGameMode)
    
public:
    ARabbitGameMode();
    virtual ~ARabbitGameMode() override = default;

    /** Actor가 게임에 배치되거나 스폰될 때 호출됩니다. */
    virtual void BeginPlay() override;

    void JudgeCapturedPhoto(UPrimitiveComponent* CapturedComp);
};

