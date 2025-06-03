#pragma once
#include "GameFramework/GameMode.h"
#include "Components/StaticMeshComponent.h"

enum class EPhotoType
{
    NONE,
    ECarrotField,
    ERabbit,
    END,
};

class ARabbitGameMode : public AGameMode
{
    DECLARE_CLASS(ARabbitGameMode, AGameMode)
    
public:
    ARabbitGameMode();
    virtual ~ARabbitGameMode() override = default;
    virtual void Tick(float DeltaTime) override;

    /** Actor가 게임에 배치되거나 스폰될 때 호출됩니다. */
    virtual void BeginPlay() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    
    void JudgeCapturedPhoto(UPrimitiveComponent* CapturedComp,RabbitCamera* RabbitCam);
    
    void StartUIDeathTimer();
    
    void ClearUIDeathTimer();
    
    void Restart();

private:
    TSet<EPhotoType> CapturedPhotoTypes;
    int32 EPhotoTypeSize;
    bool IsEndEffectOn = false;
    float EndEffectLastTime = 3.f;

    void OnPlayerDeath();
};

