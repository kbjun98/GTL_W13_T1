#include "RabbitGameUIPanel.h"
#include <EngineLoop.h>
#include <ImGui/imgui_internal.h>


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
    SetSupportedWorldTypes(EWorldTypeBitFlag::Editor | EWorldTypeBitFlag::PIE);
}

void RabbitGameUIPanel::Render()
{
    float progress = 1.0f - (FEngineLoop::PlayerCam->GetCameraCoolTime() / FEngineLoop::PlayerCam->GetCameraCoolTimeInit());

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
    snprintf(buffer, sizeof(buffer), "%.1fs", FEngineLoop::PlayerCam->GetCameraCoolTime());
    ImVec2 text_size = ImGui::CalcTextSize(buffer);
    draw_list->AddText(center - text_size * 0.5f, IM_COL32_WHITE, buffer);

}

void RabbitGameUIPanel::OnResize(HWND hWnd)
{
    RECT ClientRect;
    GetClientRect(hWnd, &ClientRect);
    // Width = ClientRect.right - ClientRect.left;
    // Height = ClientRect.bottom - ClientRect.top;
}


void RabbitGameUIPanel::RenderGallery()
{

    static bool showLargeView = false;
    static int32 selectedPhotoIndex = -1;
    static const FRenderTargetRHI* selectedPicture = nullptr;

    TArray<FRenderTargetRHI*> Pictures;
    Pictures = FEngineLoop::PlayerCam->GetPicturesRHI();

    if (Pictures.Num() <= 0)
    {
        ImGui::Text("No pictures available.");
        ImGui::Separator();
        return;
    }

    // 큰 화면 보기 모달
    if (showLargeView && selectedPicture && selectedPicture->SRV)
    {
        ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("Picture Viewer", &showLargeView, ImGuiWindowFlags_NoCollapse))
        {
            ImVec2 windowSize = ImGui::GetContentRegionAvail();
            float maxSize = FMath::Min(windowSize.x, windowSize.y) - 20.0f;

            // 중앙 배치
            ImVec2 imageSize(maxSize, maxSize);
            ImVec2 cursorPos = ImGui::GetCursorPos();
            ImVec2 centerPos = ImVec2(
                cursorPos.x + (windowSize.x - imageSize.x) * 0.5f,
                cursorPos.y + (windowSize.y - imageSize.y) * 0.5f
            );
            ImGui::SetCursorPos(centerPos);

            ImGui::Image(reinterpret_cast<ImTextureID>(selectedPicture->SRV), imageSize);

            // 닫기 버튼 (우상단)
            ImGui::SetCursorPos(ImVec2(windowSize.x - 80, 10));
            if (ImGui::Button("Close", ImVec2(70, 30)))
            {
                showLargeView = false;
                selectedPicture = nullptr;
                selectedPhotoIndex = -1;
            }
        }


        float progress = 1.0f - (FEngineLoop::PlayerCam->GetCameraCoolTime() / FEngineLoop::PlayerCam->GetCameraCoolTimeInit()); // 0~1 범위
        ImVec2 center = ImVec2(ImGui::GetCursorScreenPos().x + 40, ImGui::GetCursorScreenPos().y + 40);
        float radius = 30.0f;
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        // 배경 원
        draw_list->AddCircle(center, radius, IM_COL32(100, 100, 100, 255), 64, 2.0f);

        // 진행 원호
        draw_list->PathClear();
        int segments = 64;
        for (int i = 0; i <= segments * progress; ++i)
        {
            float angle = (i / (float)segments) * 2 * IM_PI - IM_PI / 2; // -90도부터 시작
            draw_list->PathLineTo(center + ImVec2(cosf(angle), sinf(angle)) * radius);
        }
        draw_list->PathStroke(IM_COL32(100, 255, 100, 255), false, 4.0f);

        // 텍스트 (남은 시간)
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "%.1fs", FEngineLoop::PlayerCam->GetCameraCoolTime());
        ImVec2 text_size = ImGui::CalcTextSize(buffer);
        draw_list->AddText(center - text_size * 0.5f, IM_COL32_WHITE, buffer);


        ImGui::End();
    }

    // 썸네일 설정
    constexpr float THUMBNAIL_SIZE = 128.0f;
    constexpr ImVec2 UV0(0.0f, 0.0f);
    constexpr ImVec2 UV1(1.0f, 1.0f);

    // 한 줄에 표시할 썸네일 개수 계산
    const float availableWidth = ImGui::GetContentRegionAvail().x;
    const float itemSpacing = ImGui::GetStyle().ItemSpacing.x;
    const int photosPerLine = FMath::Max(1, static_cast<int>(availableWidth / (THUMBNAIL_SIZE + itemSpacing)));

    int validPhotoCount = 0;

    for (int32 photoIdx = 0; photoIdx < Pictures.Num(); ++photoIdx)
    {
        const FRenderTargetRHI* picturePtr = Pictures[photoIdx];

        // 유효성 검사
        if (!picturePtr || !picturePtr->SRV)
        {
            continue;
        }

        // 썸네일 렌더링 (고정 크기)
        ImGui::PushID(photoIdx);

        if (ImGui::ImageButton("##thumbnail", reinterpret_cast<ImTextureID>(picturePtr->SRV), ImVec2(THUMBNAIL_SIZE, THUMBNAIL_SIZE), UV0, UV1))
        {
            // 클릭 시 큰 화면으로 보기
            showLargeView = true;
            selectedPhotoIndex = photoIdx;
            selectedPicture = picturePtr;
        }

        ImGui::PopID();

        // 줄바꿈 처리
        validPhotoCount++;
        if (validPhotoCount % photosPerLine != 0 && photoIdx < Pictures.Num() - 1)
        {
            ImGui::SameLine();
        }
    }

    // 사진이 하나도 유효하지 않은 경우
    if (validPhotoCount == 0)
    {
        ImGui::Text("No valid pictures to display.");
    }



    ImGui::Separator();
}
