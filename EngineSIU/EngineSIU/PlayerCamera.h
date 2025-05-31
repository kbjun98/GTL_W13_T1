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

    bool CanTakePicture = true;
    float TakePictureCoolTime;
    float TakePictureCoolTimeInit = 5;

public:
    void TakePicture();
    void ReleasePictures();

    TArray<FRenderTargetRHI*> GetPicturesRHI() const;
    void TriggerShutterEffect();
    void Tick(float deltaTime);

    const float GetCurrentApertureProgress() const;
    void SetCurrentApertureProgress(float value);
    void CheckObject();
};

