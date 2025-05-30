#pragma once

#include "UnrealClient.h"

class PlayerCamera
{
private:
    TArray<FRenderTargetRHI> Pictures;

public:
    void TakePicture();
};

