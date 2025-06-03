#pragma once
#include "Camera/CameraShakeBase.h"


class UHitCameraShake : public UCameraShakeBase
{
    DECLARE_CLASS(UHitCameraShake, UCameraShakeBase)

public:
    UHitCameraShake();
    virtual ~UHitCameraShake() override = default;
    
};
