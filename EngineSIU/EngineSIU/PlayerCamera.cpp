#include "PlayerCamera.h"
#include <EngineLoop.h>
#include "SoundManager.h"
#include "LevelEditor/SLevelEditor.h"
#include "UnrealEd/EditorViewportClient.h"
#include "Components/PrimitiveComponent.h"
#include <UObject/UObjectIterator.h>
#include <Actors/Player.h>
#include <Engine/Engine.h>
#include "World/World.h"
#include "GameFramework/Pawn.h"

PlayerCamera::PlayerCamera()
{
    FSoundManager::GetInstance().LoadSound("Shutter", "Contents/Rabbit/Sound/Shutter.mp3");
    FSoundManager::GetInstance().LoadSound("Error", "Contents/Rabbit/Sound/Error.mp3");
    CameraCoolTime = CameraCoolTimeInit;
}

PlayerCamera::~PlayerCamera()
{
    ReleasePictures();
}

FRenderTargetRHI* PlayerCamera::CopyRHI(FRenderTargetRHI* InputRHI)
{
    auto Device = GEngineLoop.GraphicDevice.Device;
    auto DeviceContext = GEngineLoop.GraphicDevice.DeviceContext;

    // 0. 원본 및 Graphics 객체 유효성 검사
    if (!InputRHI || !InputRHI->Texture2D)
    {
        // 필요한 정보가 없으면 nullptr 반환
        // (오류 로그를 남기는 것이 좋습니다)
        return nullptr;
    }

    // 1. 새로운 FRenderTargetRHI 객체를 힙에 할당
    FRenderTargetRHI* destination = new (std::nothrow) FRenderTargetRHI();
    if (!destination)
    {
        // 메모리 할당 실패
        return nullptr;
    }

    // 2. 원본 텍스처 디스크립션 가져오기
    D3D11_TEXTURE2D_DESC texDesc;
    InputRHI->Texture2D->GetDesc(&texDesc);

    // 3. 새로운 텍스처 생성 (깊은 복사 대상)
 
    HRESULT hr = Device->CreateTexture2D(&texDesc, nullptr, &destination->Texture2D);
    if (FAILED(hr) || !destination->Texture2D)
    {
        delete destination; // 실패 시 할당된 FRenderTargetRHI 객체 메모리 해제
        return nullptr;
    }

    // 4. 원본 텍스처 내용을 새 텍스처로 복사 (GPU 상에서 깊은 복사)
    DeviceContext->CopyResource(destination->Texture2D, InputRHI->Texture2D);

    // 5. 새로 복사된 텍스처에 대한 SRV 생성
    if (destination->Texture2D)
    {
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = texDesc.Format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = (texDesc.MipLevels == 0) ? -1 : texDesc.MipLevels; // 모든 밉 레벨 또는 명시적 값

        hr = GEngineLoop.GraphicDevice.Device->CreateShaderResourceView(destination->Texture2D, &srvDesc, &destination->SRV);
        if (FAILED(hr))
        {
            destination->Release(); // Texture2D 해제 포함
            delete destination;
            return nullptr;
        }
    }
    else // 이 경우는 거의 발생하지 않아야 함
    {
        delete destination;
        return nullptr;
    }

    // destination->RTV는 nullptr로 유지됩니다.

    // 성공적으로 깊은 복사된 FRenderTargetRHI 객체의 포인터 반환
    return destination;
}

float PlayerCamera::GetCameraCoolTime()
{
    return CameraCoolTime;
}

float PlayerCamera::GetCameraCoolTimeInit()
{
    return CameraCoolTimeInit;
}


void PlayerCamera::TakePicture()
{
    if (!CanTakePicture)
    {
        FSoundManager::GetInstance().PlaySound("Error");
        return;
    }
    auto Source =  GEngineLoop.GetLevelEditor()->GetActiveViewportClient()->GetViewportResource()->GetRenderTarget(EResourceType::ERT_DepthOfField_Result);
    
    Source = CopyRHI(Source);

    assert(Source);

    if (Source)
    {
        PicturesRHI.Add(Source);
        FSoundManager::GetInstance().PlaySound("Shutter");
        TriggerShutterEffect();
        CheckObject();
        CanTakePicture = false;
    }
}

void PlayerCamera::ReleasePictures()
{
    for (auto Picture : PicturesRHI)
    {
        Picture->Release();
    }

    PicturesRHI.Empty();
}

TArray<FRenderTargetRHI*> PlayerCamera::GetPicturesRHI() const
{
    return PicturesRHI;
}

void PlayerCamera::TriggerShutterEffect()
{
    bIsShutterAnimating = true;
    ShutterTimer = 0.0f;
    CurrentApertureProgress = 0.0f; // 시작 시점은 이미 0이거나, 여기서 명시적으로 0으로 설정
}

void PlayerCamera::Tick(float DeltaTime)
{
    if (!CanTakePicture)
    {
        CameraCoolTime -= DeltaTime;

        if (CameraCoolTime <= 0)
        {
            CanTakePicture = true;
            CameraCoolTime = CameraCoolTimeInit;
        }
    }


    if (!bIsShutterAnimating) 
    {
        // 애니메이션 중이 아니면 항상 열린 상태로 설정 (선택적)
        CurrentApertureProgress = 0.0f; 
        return;
    }

    ShutterTimer += DeltaTime;
    float NormalizedTime = ShutterTimer / ShutterDuration;

    if (NormalizedTime < 0.4f) 
    {
        CurrentApertureProgress = (NormalizedTime / 0.4f);
    }
    else if (NormalizedTime < 0.6f)
    {
        CurrentApertureProgress = 1.0f;
    }
    else if (NormalizedTime < 1.0f)
    {
        CurrentApertureProgress = 1.0f - ((NormalizedTime - 0.6f) / 0.4f);
    }
    else 
    {
        CurrentApertureProgress = 0.0f;
        bIsShutterAnimating = false;
    }

    CurrentApertureProgress = std::max(0.0f, std::min(1.0f, CurrentApertureProgress));

}

const float PlayerCamera::GetCurrentApertureProgress() const
{
    return CurrentApertureProgress;
}

void PlayerCamera::SetCurrentApertureProgress(float value)
{
    CurrentApertureProgress = value;
}

void PlayerCamera::CheckObject()
{
    auto Player = GEngine->ActiveWorld->GetMainPlayer();

    FVector PlayerForward = Player->GetActorForwardVector();
    FVector PlayerPosition = Player->GetActorLocation();


    float MaxRange = 100.f; // 최대 거리
    float FOV = FMath::Cos(FMath::DegreesToRadians(45.f)); // 느슨한 시야각 (총 30도)

    UPrimitiveComponent* bestHitComponent = nullptr;
    float minWorldHitDistance = MaxRange + 1.0f;

    for (auto currentComponent : TObjectRange<UPrimitiveComponent>()) {
        if (!currentComponent)
            continue;

        FVector ObjectLocation = currentComponent->GetComponentLocation();
        FVector ToObject = ObjectLocation - PlayerPosition;
        float DistanceToObject = ToObject.Length();

        if (DistanceToObject > MaxRange)
            continue;

        FVector ToObjectDir = ToObject.GetSafeNormal();
        float Dot = FVector::DotProduct(PlayerForward, ToObjectDir);

        if (Dot >= FOV) {
            // 시야각 안에 있고, 더 가까운 오브젝트인지 확인
            if (DistanceToObject < minWorldHitDistance) {
                minWorldHitDistance = DistanceToObject;
                bestHitComponent = currentComponent;
            }
        }
    }

    if (bestHitComponent) {
        std::cout << "Object in FOV: " << (void*)bestHitComponent
            << " at distance: " << minWorldHitDistance << std::endl;
    }
    else {
        std::cout << "No object found in loose FOV range." << std::endl;
    }
}
