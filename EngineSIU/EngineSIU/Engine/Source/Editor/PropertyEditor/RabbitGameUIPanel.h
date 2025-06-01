#pragma once

#include "UObject/ObjectMacros.h"
#include "UObject/ObjectTypes.h"

#include "UnrealEd/EditorPanel.h"

class RabbitGameUIPanel : public UEditorPanel
{
public:
    RabbitGameUIPanel();
    
    void Render() override;
private:
    void OnResize(HWND hWnd) override;
    void RenderGallery();
    bool RegisterPlayerCamera();

    std::shared_ptr<PlayerCamera> PlayerCam;

    bool showLargeView = false;
    int32 selectedPhotoIndex = -1;
    const FRenderTargetRHI* selectedPicture = nullptr;

};

