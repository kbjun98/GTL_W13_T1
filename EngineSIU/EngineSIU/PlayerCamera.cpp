#include "PlayerCamera.h"
#include <EngineLoop.h>
#include "PostProcess/PostProcessRenderPass.h"
#include "PostProcess/DepthOfFieldRenderPass.h"
#include "SoundManager.h"

PlayerCamera::PlayerCamera()
{
    FSoundManager::GetInstance().LoadSound("Shutter", "Contents/Rabbit/Shutter.mp3");
}

void PlayerCamera::TakePicture()
{
    auto Source = FEngineLoop::Renderer.GetPostProcessRenderPass()->GetDepthOfFieldRenderPass()->GetPostProcessSource();

    if (Source)
    {
        Pictures.Add(Source);
        FSoundManager::GetInstance().PlaySound("Shutter");
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
