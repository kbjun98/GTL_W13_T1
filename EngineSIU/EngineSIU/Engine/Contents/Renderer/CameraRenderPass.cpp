#include "CameraRenderPass.h"

#include "RendererHelpers.h"
#include "D3D11RHI/DXDShaderManager.h"
#include "D3D11RHI/GraphicDevice.h"
#include "Engine/Engine.h"
#include "GameFramework/RabbitPlayer.h"
#include "UnrealEd/EditorViewportClient.h"
#include "World/World.h"

void FCameraRenderPass::Initialize(FDXDBufferManager* InBufferManager, FGraphicsDevice* InGraphics, FDXDShaderManager* InShaderManager)
{
    FRenderPassBase::Initialize(InBufferManager, InGraphics, InShaderManager);
}

void FCameraRenderPass::PrepareRenderArr()
{
    if (GEngine->ActiveWorld->WorldType == EWorldType::PIE)
    {
        if (APlayerController* PC = GEngine->ActiveWorld->GetPlayerController())
        {
            if (ARabbitPlayer* Rabbit = Cast<ARabbitPlayer>(PC->GetPawn()))
            {
                if (Rabbit->IsADS())
                {
                    bShouldRender = true;
                    return;
                }
            }
        }
    }
    bShouldRender = false;
}

void FCameraRenderPass::ClearRenderArr()
{
    
}

void FCameraRenderPass::Render(const std::shared_ptr<FEditorViewportClient>& Viewport)
{
    PrepareRender(Viewport);

    if (bShouldRender)
    {
        auto Texture = FEngineLoop::ResourceManager.GetTexture(L"Contents/Texture/CameraViewFinder.png");
        Graphics->DeviceContext->PSSetShaderResources(0, 1, &Texture->TextureSRV);
    }
    else
    {
        ID3D11ShaderResourceView* NullSRV[1] = { nullptr };
        Graphics->DeviceContext->PSSetShaderResources(0, 1, NullSRV);
    }
    
    FQuadTransform Transform;
    BufferManager->UpdateConstantBuffer<FQuadTransform>("FSlateTransform", Transform);
    Graphics->DeviceContext->Draw(6, 0);

    CleanUpRender(Viewport);
}

void FCameraRenderPass::PrepareRender(const std::shared_ptr<FEditorViewportClient>& Viewport)
{
    constexpr EResourceType ResourceType = EResourceType::ERT_CameraW13; 
    FRenderTargetRHI* RenderTargetRHI = Viewport->GetViewportResource()->GetRenderTarget(ResourceType);
    Graphics->DeviceContext->OMSetRenderTargets(1, &RenderTargetRHI->RTV, nullptr);
    
    Graphics->DeviceContext->RSSetState(Graphics->RasterizerSolidBack);
    Graphics->DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    
    Graphics->DeviceContext->IASetInputLayout(nullptr);
    
    ID3D11VertexShader* VertexShader = ShaderManager->GetVertexShaderByKey(L"W13CameraVertex");
    Graphics->DeviceContext->VSSetShader(VertexShader, nullptr, 0);

    ID3D11PixelShader* PixelShader = ShaderManager->GetPixelShaderByKey(L"W13CameraPixel");
    Graphics->DeviceContext->PSSetShader(PixelShader, nullptr, 0);

    BufferManager->BindConstantBuffer(TEXT("FSlateTransform"), 0, EShaderStage::Vertex);

    // Resource
    FRenderTargetRHI* CompositeResultRHI = Viewport->GetViewportResource()->GetRenderTarget(EResourceType::ERT_Compositing);
    Graphics->DeviceContext->PSSetShaderResources(1, 1, &CompositeResultRHI->SRV);
}

void FCameraRenderPass::CleanUpRender(const std::shared_ptr<FEditorViewportClient>& Viewport)
{
    Graphics->DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

    ID3D11ShaderResourceView* NullSRV[1] = { nullptr };
    Graphics->DeviceContext->PSSetShaderResources(0, 1, NullSRV);
    Graphics->DeviceContext->PSSetShaderResources(1, 1, NullSRV);
}

void FCameraRenderPass::CreateResource()
{
    HRESULT hr = ShaderManager->AddVertexShader(L"W13CameraVertex", L"Shaders/W13/W13CameraShader.hlsl", "mainVS");
    if (FAILED(hr))
    {
        MessageBox(nullptr, L"Failed to load vertex shader", L"Error", MB_OK);
    }

    hr = ShaderManager->AddPixelShader(L"W13CameraPixel", L"Shaders/W13/W13CameraShader.hlsl", "mainPS");
    if (FAILED(hr))
    {
        MessageBox(nullptr, L"Failed to load Pixel shader", L"Error", MB_OK);
    }
}
