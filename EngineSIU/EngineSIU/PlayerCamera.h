#pragma once

#include "UnrealClient.h"

class PlayerCamera
{
private:

    PlayerCamera();

    TArray<FRenderTargetRHI*> Pictures;

public:
    void TakePicture();
    void ReleasePictures();

    TArray<FRenderTargetRHI*> GetPictures();
};

