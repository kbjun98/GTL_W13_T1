#pragma once

#include "UObject/ObjectMacros.h"
#include "UObject/ObjectTypes.h"

#include "UnrealEd/EditorPanel.h"
#include <RabbitCamera.h>

class RabbitGameUIPanel : public UEditorPanel
{
public:
    RabbitGameUIPanel();
    
    void Render() override;
private:
    void OnResize(HWND hWnd) override;
    void RenderGallery();
    bool RegisterPlayerCamera();

    float Width;
    float Height;

    std::shared_ptr<RabbitCamera> PlayerCam;

    bool showLargeView = false;
    int32 selectedPhotoIndex = -1;
    const FRenderTargetRHI* selectedPicture = nullptr;

};

