#pragma once

#include "UObject/ObjectMacros.h"
#include "UObject/ObjectTypes.h"

#include "UnrealEd/EditorPanel.h"
#include <RabbitCamera.h>

class RabbitGameUIPanel : public UEditorPanel
{
public:
    RabbitGameUIPanel();
    
    virtual void Render() override;
    
    void Restart();
    
    void StartDeathTimer();

    void ClearDeathTimer();
    
    void RenderDeathUI();
    
private:
    virtual void OnResize(HWND hWnd) override;
    
    void RenderGallery();
    
    bool RegisterPlayerCamera();
    
    void RenderCameraCool();
    
    float Width;
    float Height;

    bool bShowDeathUI = false;
    bool bDeathTriggered = false;  // 죽음이 트리거되었는지
    float deathUIDelay = 4.0f;     // 지연 시간 (초)
    float DeathTimer = 0.0f;       // 현재 타이머

    std::shared_ptr<RabbitCamera> PlayerCam;

    bool showLargeView = false;
    int32 selectedPhotoIndex = -1;
    const FRenderTargetRHI* selectedPicture = nullptr;

};

