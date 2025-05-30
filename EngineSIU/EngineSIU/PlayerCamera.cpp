#include "PlayerCamera.h"
#include <EngineLoop.h>
#include "PostProcess/PostProcessRenderPass.h"
#include "PostProcess/DepthOfFieldRenderPass.h"
#include "SoundManager.h"

PlayerCamera::PlayerCamera()
{
    FSoundManager::GetInstance().LoadSound("Shutter", "Contents/Rabbit/Sound/Shutter.mp3");
}

void PlayerCamera::TakePicture()
{
    auto Source = FEngineLoop::Renderer.GetPostProcessRenderPass()->GetDepthOfFieldRenderPass()->GetPostProcessSource();

    if (Source)
    {
        Pictures.Add(Source);
        FSoundManager::GetInstance().PlaySound("Shutter");
        TriggerShutterEffect();
    }
}

void PlayerCamera::ReleasePictures()
{
    for (auto Picture : Pictures)
    {
        Picture->Release();
    }
}

TArray<FRenderTargetRHI*> PlayerCamera::GetPictures()
{
    return Pictures;
}

void PlayerCamera::TriggerShutterEffect()
{
    bIsShutterAnimating = true;
    ShutterTimer = 0.0f;
    CurrentApertureProgress = 0.0f; // 시작 시점은 이미 0이거나, 여기서 명시적으로 0으로 설정
}

void PlayerCamera::UpdateShutterAnimation(float deltaTime)
{
    if (!bIsShutterAnimating) {
        // 애니메이션 중이 아니면 항상 열린 상태로 설정 (선택적)
        CurrentApertureProgress = 0.0f; 
        return;
    }

    ShutterTimer += deltaTime/1.5f;
    float normalizedTime = ShutterTimer / ShutterDuration;

    if (normalizedTime < 0.4f) {
        CurrentApertureProgress = (normalizedTime / 0.4f);
    }
    else if (normalizedTime < 0.6f) {
        CurrentApertureProgress = 1.0f;
    }
    else if (normalizedTime < 1.0f) {
        CurrentApertureProgress = 1.0f - ((normalizedTime - 0.6f) / 0.4f);
    }
    else {
        CurrentApertureProgress = 0.0f;
        bIsShutterAnimating = false;
    }
    CurrentApertureProgress = std::max(0.0f, std::min(1.0f, CurrentApertureProgress));

    std::cout << CurrentApertureProgress << std:: endl;
}

float PlayerCamera::GetCurrentApertureProgress()
{
    return CurrentApertureProgress;
}

void PlayerCamera::SetCurrentApertureProgress(float value)
{
    CurrentApertureProgress = value;
}
