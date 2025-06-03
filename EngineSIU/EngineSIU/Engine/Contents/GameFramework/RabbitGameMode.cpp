#include "RabbitGameMode.h"
#include "Engine/Contents/GameFramework/RabbitPawn.h"
#include "Engine/Contents/GameFramework/RabbitPlayer.h"
#include "Engine/Contents/GameFramework/RabbitController.h"
#include <Engine/Engine.h>
#include "World/World.h"
#include "UnrealEd/UnrealEd.h"
#include <PropertyEditor/RabbitGameUIPanel.h>
#include "SoundManager.h"

ARabbitGameMode::ARabbitGameMode()
{
    // 기본 플레이어 Pawn 클래스 설정
    DefaultPawnClass = ARabbitPlayer::StaticClass();

    // 플레이어 컨트롤러 클래스 설정
    PlayerControllerClass = ARabbitController::StaticClass();

    EPhotoTypeSize = static_cast<int>(EPhotoType::END) - 1;
}

void ARabbitGameMode::Tick(float DeltaTime)
{
    if (IsEndEffectOn)
    {
        EndEffectLastTime -= DeltaTime*3.3f;

        if (EndEffectLastTime <= 0)
        {
            IsEndEffectOn = false;
            FEngineLoop::TimeScale = 1.0f;
        }
    }
}

void ARabbitGameMode::BeginPlay()
{
    Super::BeginPlay();
 
    if (APlayerController* PlayerController = GEngine->ActiveWorld->GetPlayerController())
    {
        if (ARabbitPlayer* Rabbit = Cast<ARabbitPlayer>(PlayerController->GetPawn()))
        {
            Rabbit->OnPlayerDied.BindLambda(
                [this]
                {
                    StartUIDeathTimer();
                }
            );

            if (std::shared_ptr<RabbitCamera> Camera = Rabbit->GetRabbitCamera())
            {
                Camera->InitPictureArraySize(EPhotoTypeSize);

                Camera->OnPictureTaken.BindLambda(
                    [this](RabbitCamera* RabbitCamera, UPrimitiveComponent* Comp, FVector PlayerLocation) 
                    {
                        this->JudgeCapturedPhoto(Comp, RabbitCamera);
                    }
                );
            }
        }
    }
}

void ARabbitGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    ClearUIDeathTimer();
}

void ARabbitGameMode::JudgeCapturedPhoto(UPrimitiveComponent* CapturedComp, RabbitCamera* RabbitCam)
{
    if (UStaticMeshComponent* Comp = Cast<UStaticMeshComponent>(CapturedComp))
    {
        EPhotoType CapturedType = Comp->GetPhotoType();

        // 무효한 타입 무시
        if (CapturedType <= EPhotoType::NONE || CapturedType >= EPhotoType::END)
        {
            return;
        }

        // 중복 방지
        if (!CapturedPhotoTypes.Contains(CapturedType))
        {
            CapturedPhotoTypes.Add(CapturedType);

            RabbitCam->StorePicture(CapturedType);
            // 로그 출력 (선택)
            std::cout << "Captured: " << static_cast<int>(CapturedType) << std::endl;

        }
    }

    bool IsEnd = CapturedPhotoTypes.Num() == EPhotoTypeSize;

    RabbitCam->PlayCameraSound(IsEnd);

    if (IsEnd)
    {
        IsEndEffectOn=true;
        FEngineLoop::TimeScale = .3f;
    }
   
}

void ARabbitGameMode::StartUIDeathTimer()
{
    auto Panel = GEngineLoop.GetUnrealEditor()->GetEditorPanel("RabbitGameUIPanel");
    auto RabbitPanel = std::dynamic_pointer_cast<RabbitGameUIPanel>(Panel);
    
    RabbitPanel->StartDeathTimer();
    
    FSoundManager::GetInstance().PlaySound("GameOver");
}

void ARabbitGameMode::ClearUIDeathTimer()
{
    auto Panel = GEngineLoop.GetUnrealEditor()->GetEditorPanel("RabbitGameUIPanel");
    auto RabbitPanel = std::dynamic_pointer_cast<RabbitGameUIPanel>(Panel);
    
    RabbitPanel->ClearDeathTimer();
}
