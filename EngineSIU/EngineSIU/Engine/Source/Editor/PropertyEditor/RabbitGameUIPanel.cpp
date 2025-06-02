#include "RabbitGameUIPanel.h"
#include <EngineLoop.h>
#include <ImGui/imgui_internal.h>
#include <Engine/Contents/GameFramework/RabbitPawn.h>
#include <UObject/UObjectIterator.h>
#include "Engine/Contents/GameFramework/RabbitPlayer.h"
#include <Engine/Engine.h>


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

void RabbitGameUIPanel::Render()
{

    if (!RegisterPlayerCamera())
    {
        return;
    }
    RenderCameraCool();
   RenderGallery();
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
