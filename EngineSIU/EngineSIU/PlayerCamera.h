#pragma once

#include "UnrealClient.h"

class PlayerCamera
{
public:
    PlayerCamera();

private:
    TArray<FRenderTargetRHI*> Pictures;

public:
    void TakePicture();
    void ReleasePictures();

    TArray<FRenderTargetRHI*> GetPictures();
};

