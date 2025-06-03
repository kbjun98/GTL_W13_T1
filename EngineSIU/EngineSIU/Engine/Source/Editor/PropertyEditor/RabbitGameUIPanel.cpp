#include "RabbitGameUIPanel.h"
#include <EngineLoop.h>
#include <ImGui/imgui_internal.h>
#include <Engine/Contents/GameFramework/RabbitPawn.h>
#include <UObject/UObjectIterator.h>
#include "Engine/Contents/GameFramework/RabbitPlayer.h"
#include <Engine/Engine.h>
#include "World/World.h"
#include "LevelEditor/SLevelEditor.h"
#include "Editor/UnrealEd/EditorViewportClient.h"
#include "GameFramework/RabbitGameMode.h"

inline ImVec2 operator*(const ImVec2& lhs, float rhs) {
    return ImVec2(lhs.x * rhs, lhs.y * rhs);
}
inline ImVec2 operator*(float lhs, const ImVec2& rhs) {
    return ImVec2(lhs * rhs.x, lhs * rhs.y);
}
inline ImVec2 operator+(const ImVec2& a, const ImVec2& b) {
    return ImVec2(a.x + b.x, a.y + b.y);
}
inline ImVec2 operator-(const ImVec2& a, const ImVec2& b) {
    return ImVec2(a.x - b.x, a.y - b.y);
}

RabbitGameUIPanel::RabbitGameUIPanel()
{
    SetSupportedWorldTypes(EWorldTypeBitFlag::PIE);
}



void RabbitGameUIPanel::ShowBouncingWindow(float DeltaTime)
{
    constexpr float downDuration = 1.2f;
    constexpr float waitDuration = 2.5f;
    constexpr float upDuration = 0.8f;

    switch (bounceState)
    {
    case BounceState::Idle:
        bounce.Start(downDuration, &BounceTween::EaseOutBounce);
        bounceState = BounceState::Down;
        break;
    case BounceState::Down:
        if (!bounce.IsPlaying()) {
            bounceState = BounceState::Wait;
            waitTimer = 0.0f;
        }
        break;
    case BounceState::Wait:
        waitTimer += DeltaTime;
        if (waitTimer >= waitDuration) {
            bounce.Start(upDuration, &BounceTween::EaseInBack);
            bounceState = BounceState::Up;
        }
        break;
    case BounceState::Up:
        if (!bounce.IsPlaying()) {
            bounceState = BounceState::Done;
        }
        break;
    case BounceState::Done:
        // 애니메이션 종료
        break;
    }

    // 네가 준 코드에서 뷰포트 정보 받아서 중심 계산
    auto ViewPort = GEngineLoop.GetLevelEditor()->GetActiveViewportClient()->GetViewport()->GetD3DViewport();
    ImVec2 center(
        ViewPort.TopLeftX + ViewPort.Width * 0.5f,
        ViewPort.TopLeftY + ViewPort.Height * 0.4f
    );

    ImVec2 windowSize(300, 120);
    float currentX = 0.0f;

    if (bounceState == BounceState::Down)
    {
        float t = bounce.Update(DeltaTime);
        float startX = ViewPort.TopLeftX - windowSize.x - 100.0f; // 화면 왼쪽 밖
        float endX = center.x; // 화면 중앙
        currentX = startX + (endX - startX) * t;
    }
    else if (bounceState == BounceState::Wait)
    {
        currentX = center.x; // 중앙에서 대기
    }
    else if (bounceState == BounceState::Up)
    {
        float t = bounce.Update(DeltaTime);
        float startX = center.x; // 중앙에서 시작
        float endX = ViewPort.TopLeftX + ViewPort.Width + windowSize.x + 300.0f; // 화면 오른쪽 멀리
        currentX = startX + (endX - startX) * t;
    }
    else if (bounceState == BounceState::Idle)
    {
        currentX = ViewPort.TopLeftX - windowSize.x - 100.0f; // 화면 왼쪽 밖
    }
    else if (bounceState == BounceState::Done)
    {
        currentX = ViewPort.TopLeftX + ViewPort.Width + windowSize.x + 300.0f; // Up과 같은 위치로 고정
    }

    ImVec2 windowPos(currentX - windowSize.x * 0.5f, center.y - windowSize.y * 0.5f);

    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);

    ImTextureID textureID = (ImTextureID)FEngineLoop::ResourceManager.GetTexture(L"Assets/Texture/DeathBG.png")->TextureSRV;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 20.0f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));

    ImGui::Begin("📷 Bounced", nullptr,
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoTitleBar);

    ImVec2 imagePos = ImGui::GetWindowPos();
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    drawList->AddImage(
        textureID,
        imagePos,
        ImVec2(imagePos.x + windowSize.x, imagePos.y + windowSize.y),
        ImVec2(0, 0),
        ImVec2(1, 1)
    );

    ImVec2 textPos(
        (windowSize.x - ImGui::CalcTextSize("모든 사진을 찍었습니다!").x) * 0.5f,
        windowSize.y * 0.4f
    );

    ImVec2 bgMin = ImGui::GetWindowPos() + textPos - ImVec2(5, 3);
    ImVec2 bgMax = bgMin + ImGui::CalcTextSize("모든 사진을 찍었습니다!") + ImVec2(10, 6);

    // 배경 사각형 (검은색, 반투명)
    drawList->AddRectFilled(bgMin, bgMax, IM_COL32(0, 0, 0, 150), 5.0f);

    ImGui::SetCursorPos(textPos);
    ImGui::Text("모든 사진을 찍었습니다!");

    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
}

void RabbitGameUIPanel::Restart()
{
    ClearDeathTimer();

    if (ARabbitGameMode* RabbitGameMode = Cast<ARabbitGameMode>(GEngine->ActiveWorld->GetGameMode()))
    {
        RabbitGameMode->Restart();
    }
}

void RabbitGameUIPanel::StartDeathTimer()
{
    bDeathTriggered = true;  // 죽음 트리거
    DeathTimer = 0.0f;       // 타이머 리셋
    bShowDeathUI = false;    // 아직 UI는 보이지 않음
}

void RabbitGameUIPanel::ClearDeathTimer()
{
    bDeathTriggered = false;
    DeathTimer = 0.0f;
    bShowDeathUI = false;
}

void RabbitGameUIPanel::RenderDeathUI()
{
    if (!bDeathTriggered)
    {
        return;
    }
    
    // 죽음이 트리거되었고 아직 UI가 표시되지 않았다면
    if (bDeathTriggered && !bShowDeathUI)
    {
        DeathTimer += FEngineLoop::DeltaTime;

        // 지연 시간이 지나면 UI 표시
        if (DeathTimer >= deathUIDelay)
        {
            bShowDeathUI = true;
        }
    }

    if (!bShowDeathUI)
    {
        return;
    }
    
    auto ViewPort = GEngineLoop.GetLevelEditor()->GetActiveViewportClient()->GetViewport()->GetD3DViewport();
    // 뷰포트 중심 계산
    ImVec2 centerPos(
        ViewPort.TopLeftX + ViewPort.Width * 0.5f,
        ViewPort.TopLeftY + ViewPort.Height * 0.5f
    );
    // 창 크기 및 위치
    ImVec2 windowSize(1536, 1150);
    ImVec2 windowPos(
        centerPos.x - windowSize.x * 0.5f,
        centerPos.y - windowSize.y * 0.45f
    );
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
    // 스타일 설정
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 15.0f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0)); // 완전 투명
    ImGui::Begin("💀 DeathUI", nullptr,
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoScrollbar);
    ImVec2 imagePos = ImGui::GetWindowPos();
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImTextureID textureID = (ImTextureID)FEngineLoop::ResourceManager.GetTexture(L"Assets/Texture/DeathBG.png")->TextureSRV;
    // 배경 이미지 그리기
    drawList->AddImage(textureID,
        imagePos,
        ImVec2(imagePos.x + windowSize.x, imagePos.y + windowSize.y),
        ImVec2(0, 0), ImVec2(1, 1)
    );

  
    // 커스텀 이미지 버튼
    ImVec2 buttonSize(250, 100);
    ImVec2 buttonPos(
        imagePos.x + (windowSize.x - buttonSize.x) * 0.5f,
        imagePos.y + windowSize.y - 200.f
    );

    // 버튼 배경 이미지 그리기
    ImTextureID buttonTextureID = (ImTextureID)FEngineLoop::ResourceManager.GetTexture(L"Assets/Texture/RestartButton.png")->TextureSRV;

    // 마우스 호버 체크
    ImVec2 mousePos = ImGui::GetMousePos();
    bool isHovered = (mousePos.x >= buttonPos.x && mousePos.x <= buttonPos.x + buttonSize.x &&
        mousePos.y >= buttonPos.y && mousePos.y <= buttonPos.y + buttonSize.y);

    // 호버 상태에 따른 색상 조정 (선택사항)
    ImU32 buttonTint = isHovered ? IM_COL32(255, 255, 255, 200) : IM_COL32(255, 255, 255, 255);

    drawList->AddImage(buttonTextureID,
        buttonPos,
        ImVec2(buttonPos.x + buttonSize.x, buttonPos.y + buttonSize.y),
        ImVec2(0, 0), ImVec2(1, 1),
        buttonTint
    );

    // 클릭 감지
    if (isHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        Restart();
    }
    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
}

void RabbitGameUIPanel::OnResize(HWND hWnd)
{
    RECT ClientRect;
    GetClientRect(hWnd, &ClientRect);
    Width = ClientRect.right - ClientRect.left;
    Height = ClientRect.bottom - ClientRect.top;
}


void RabbitGameUIPanel::RenderGallery()
{
    TArray<FRenderTargetRHI*> Pictures = PlayerCam->GetPicturesRHI();

    constexpr float THUMBNAIL_SIZE = 128.0f;
    constexpr float spacing = 15.0f;
    constexpr int32 MaxSlots = 3;
    constexpr float totalWidth = (THUMBNAIL_SIZE * MaxSlots) + (spacing * (MaxSlots - 1))+40.f;
    constexpr float panelHeight = THUMBNAIL_SIZE +30.f;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 panelPos = ImVec2(
        viewport->WorkPos.x + (viewport->WorkSize.x - totalWidth) * 0.4f,
        viewport->WorkPos.y + viewport->WorkSize.y - panelHeight - 5.0f // 바닥에서 살짝 띄움
    );

    ImGui::SetNextWindowPos(panelPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(totalWidth, panelHeight));
    ImGui::Begin("PhotoSlots", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoFocusOnAppearing);

    for (int32 slotIdx = 0; slotIdx < MaxSlots; ++slotIdx)
    {
        ImGui::PushID(slotIdx);

        if (slotIdx < Pictures.Num())
        {
            const FRenderTargetRHI* picturePtr = Pictures[slotIdx];
            if (picturePtr && picturePtr->SRV)
            {
                if (ImGui::ImageButton("##thumbnail", reinterpret_cast<ImTextureID>(picturePtr->SRV), ImVec2(THUMBNAIL_SIZE, THUMBNAIL_SIZE)))
                {
                    showLargeView = true;
                    selectedPhotoIndex = slotIdx;
                    selectedPicture = picturePtr;
                }
            }
            else
            {
                ImGui::Button("Empty", ImVec2(THUMBNAIL_SIZE, THUMBNAIL_SIZE));
            }
        }
        else
        {
            ImGui::Button("Empty", ImVec2(THUMBNAIL_SIZE, THUMBNAIL_SIZE));
        }

        ImGui::PopID();

        if (slotIdx < MaxSlots - 1)
        {
            ImGui::SameLine(0.0f, spacing);
        }
    }

    ImGui::End();

    // 확대 보기 모달
    if (showLargeView && selectedPicture && selectedPicture->SRV)
    {
        ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("Picture Viewer", &showLargeView, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
        {
            ImVec2 available = ImGui::GetContentRegionAvail();
            float size = FMath::Min(available.x, available.y);

            ImVec2 centerPos = ImVec2(
                (available.x - size) * 0.45f,
                (available.y - size) * 0.45f
            );
            ImGui::SetCursorPos(centerPos);
            ImGui::Image(reinterpret_cast<ImTextureID>(selectedPicture->SRV), ImVec2(size, size));

            // 닫기 버튼
            if (ImGui::Button("Close"))
            {
                showLargeView = false;
                selectedPicture = nullptr;
                selectedPhotoIndex = -1;
            }
        }
        ImGui::End();
    }
}


bool RabbitGameUIPanel::RegisterPlayerCamera()
{
    bool Registered = false;

    for (auto Rabbit : TObjectRange<ARabbitPlayer>())
    {
        if (Rabbit->GetRabbitCamera())
        {
            PlayerCam = Rabbit->GetRabbitCamera();
            Registered = true;
        }
    }

    return Registered;
}

void RabbitGameUIPanel::RenderCameraCool()
{

    float progress = 1.0f - (PlayerCam->GetCameraCoolTime() / PlayerCam->GetCameraCoolTimeInit());

    ImVec2 screen_size = ImGui::GetIO().DisplaySize;
    float radius = 60.0f;
    ImVec2 padding(40.0f, 40.0f);
    ImVec2 center = ImVec2(radius + padding.x, screen_size.y - radius - padding.y);

    ImDrawList* draw_list = ImGui::GetBackgroundDrawList(); // 또는 ForegroundDrawList

    // 배경 원
    draw_list->AddCircle(center, radius, IM_COL32(100, 100, 100, 255), 64, 2.0f);

    // 진행 원호
    draw_list->PathClear();
    int segments = 64;
    for (int i = 0; i <= segments * progress; ++i)
    {
        float angle = (i / (float)segments) * 2 * IM_PI - IM_PI / 2;
        draw_list->PathLineTo(center + ImVec2(cosf(angle), sinf(angle)) * radius);
    }
    draw_list->PathStroke(IM_COL32(100, 255, 100, 255), false, 4.0f);

    // 텍스트 (남은 시간)
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%.1fs", PlayerCam->GetCameraCoolTime());
    ImVec2 text_size = ImGui::CalcTextSize(buffer);
    draw_list->AddText(center - text_size * 0.5f, IM_COL32_WHITE, buffer);


}

void RabbitGameUIPanel::Render()
{

    if (!RegisterPlayerCamera())
    {
        return;
    }

    RenderDeathUI();
    RenderCameraCool();
    RenderGallery();
    ShowBouncingWindow(FEngineLoop::DeltaTime);
}
