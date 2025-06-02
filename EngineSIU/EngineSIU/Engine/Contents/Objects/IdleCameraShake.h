#pragma once
#include "Camera/CameraShakeBase.h"


class UIdleCameraShake : public UCameraShakeBase
{
    DECLARE_CLASS(UIdleCameraShake, UCameraShakeBase)

public:
    UIdleCameraShake();
    virtual ~UIdleCameraShake() override = default;
    
};
