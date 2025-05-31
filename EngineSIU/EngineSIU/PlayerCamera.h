#pragma once

#include "UnrealClient.h"

class PlayerCamera
{
public:
    PlayerCamera();
    ~PlayerCamera();

private:
    TArray<FRenderTargetRHI*> PicturesRHI;
    bool  bIsShutterAnimating = false;
    float ShutterTimer = 0.0f;
    const float ShutterDuration = 0.2f; // 예: 0.25초
    float CurrentApertureProgress = 0.0f;

public:
    void TakePicture();
    void ReleasePictures();

    TArray<FRenderTargetRHI*> GetPicturesRHI() const;
    void TriggerShutterEffect();
    void UpdateShutterAnimation(float deltaTime);

    const float GetCurrentApertureProgress() const;
    void SetCurrentApertureProgress(float value);
};

