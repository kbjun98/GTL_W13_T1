#pragma once

#include "UnrealClient.h"

class PlayerCamera
{
public:
    PlayerCamera();

private:
    TArray<FRenderTargetRHI*> Pictures;
    bool  bIsShutterAnimating = false;
    float ShutterTimer = 0.0f;
    float ShutterDuration = 0.25f; // 예: 0.25초
    float CurrentApertureProgress = 0.0f;

public:
    void TakePicture();
    void ReleasePictures();

    TArray<FRenderTargetRHI*> GetPictures();
    void TriggerShutterEffect();
    void UpdateShutterAnimation(float deltaTime);

    float GetCurrentApertureProgress();
    void SetCurrentApertureProgress(float value);
};

