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

    /** Actor가 게임에 배치되거나 스폰될 때 호출됩니다. */
    virtual void BeginPlay() override;
    void JudgeCapturedPhoto(UPrimitiveComponent* CapturedComp,RabbitCamera* RabbitCam);

    TSet<EPhotoType> CapturedPhotoTypes;
    int32 EPhotoTypeSize;
};

