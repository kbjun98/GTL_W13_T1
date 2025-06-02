#pragma once
#include "UnrealClient.h"
#include "Delegates/DelegateCombination.h"

class UPrimitiveComponent;
class RabbitCamera;
enum class EPhotoType;

DECLARE_DELEGATE_TwoParams(FOnPictureTaken, UPrimitiveComponent*, RabbitCamera*);


class RabbitCamera
{
public:
    RabbitCamera();
    ~RabbitCamera();

private:
    TArray<FRenderTargetRHI*> PicturesRHI;
    bool  bIsShutterAnimating = false;
    float ShutterTimer = 0.0f;
    const float ShutterDuration = 0.2f; // 예: 0.25초
    float CurrentApertureProgress = 0.0f;

    bool CanTakePicture = true;
    float CameraCoolTime;
    float CameraCoolTimeInit = 3;

public:
    void ReleasePictures();
    void Tick(float deltaTime);


    float GetCameraCoolTime();
    float GetCameraCoolTimeInit();
    const float GetCurrentApertureProgress() const;
    void SetCurrentApertureProgress(float value);
    void InitPictureArraySize(int Size);
    void TakePicture();

    TArray<FRenderTargetRHI*> GetPicturesRHI() const;

    FOnPictureTaken OnPictureTaken;
    void StorePicture(EPhotoType Type);
private:
    FRenderTargetRHI* CopySource(FRenderTargetRHI* InputRHI);
    FRenderTargetRHI* CaptureFrame();
    UPrimitiveComponent* CheckSubject();
    void TriggerShutterEffect();
    bool ValidateTakePicture();
    
};

