#pragma once
#include "RenderPassBase.h"

struct FQuadTransform
{
    FVector2D Scale = FVector2D(1.0f, 1.0f);
    FVector2D Offset = FVector2D(0.0f, 0.0f);
};

class FCameraRenderPass : public FRenderPassBase
{
public:
    FCameraRenderPass() = default;
    virtual ~FCameraRenderPass() override = default;
    
    virtual void Initialize(FDXDBufferManager* InBufferManager, FGraphicsDevice* InGraphics, FDXDShaderManager* InShaderManager) override;
    virtual void PrepareRenderArr() override;
    virtual void ClearRenderArr() override;
    virtual void Render(const std::shared_ptr<FEditorViewportClient>& Viewport) override;

protected:
    virtual void PrepareRender(const std::shared_ptr<FEditorViewportClient>& Viewport) override;
    virtual void CleanUpRender(const std::shared_ptr<FEditorViewportClient>& Viewport) override;

    virtual void CreateResource() override;

    bool bShouldRender = false;
};
