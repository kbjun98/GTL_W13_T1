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

    EndEffectLastTime = EndEffectLastTimeInit;
}

void ARabbitGameMode::Tick(float DeltaTime)
{
    if (IsEndEffectOn)
    {
        EndEffectLastTime -= DeltaTime*3.3f;

        if (EndEffectLastTime <= 0)
        {
            EndEffectLastTime = EndEffectLastTimeInit;
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
                    this->OnPlayerDeath();
                }
            );

            Rabbit->OnPlayerSucceed.BindLambda(
                [this]
                {
                    this->OnPlayerSucceed();
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

    bool WasAlreadyComplete = IsPictureComplete;
    bool IsNowComplete = CapturedPhotoTypes.Num() == EPhotoTypeSize;

    // 사진 사운드 재생
    if (!WasAlreadyComplete)
    {
        RabbitCam->PlayCameraSound(IsNowComplete);
    }
    else
    {
        RabbitCam->PlayCameraSound(false); // 그냥 일반 셔터음 또는 안 나게 하고 싶으면 생략
    }

    // 처음으로 완성된 경우에만 효과 발동
    if (!WasAlreadyComplete && IsNowComplete)
    {
        IsPictureComplete = true;
        IsEndEffectOn = true;
        FEngineLoop::TimeScale = 0.3f;
        StartUIPictureEnd();
    }
   
}

void ARabbitGameMode::StartUIPictureEnd()
{
    auto Panel = GEngineLoop.GetUnrealEditor()->GetEditorPanel("RabbitGameUIPanel");
    auto RabbitPanel = std::dynamic_pointer_cast<RabbitGameUIPanel>(Panel);

    RabbitPanel->OnPictureEndUI();
    FSoundManager::GetInstance().StopAllSounds();
    FSoundManager::GetInstance().PlaySound("Hurry");
    FSoundManager::GetInstance().PlaySound("Shoong");

    if (APlayerController* PlayerController = GEngine->ActiveWorld->GetPlayerController())
    {
        if (ARabbitPlayer* Rabbit = Cast<ARabbitPlayer>(PlayerController->GetPawn()))
        {
            Rabbit->SetIsCaptureAll(true);
        }
    }
}

void ARabbitGameMode::StartUIDeathTimer()
{
    auto Panel = GEngineLoop.GetUnrealEditor()->GetEditorPanel("RabbitGameUIPanel");
    auto RabbitPanel = std::dynamic_pointer_cast<RabbitGameUIPanel>(Panel);
    
    RabbitPanel->StartDeathTimer();
    FSoundManager::GetInstance().StopAllSounds();
    FSoundManager::GetInstance().PlaySound("GameOver");
}

void ARabbitGameMode::ClearUIDeathTimer()
{
    auto Panel = GEngineLoop.GetUnrealEditor()->GetEditorPanel("RabbitGameUIPanel");
    auto RabbitPanel = std::dynamic_pointer_cast<RabbitGameUIPanel>(Panel);
    
    RabbitPanel->ClearDeathTimer();
}

void ARabbitGameMode::OnPlayerDeath()
{

    StartUIDeathTimer();
}

void ARabbitGameMode::OnPlayerSucceed()
{
    auto Panel = GEngineLoop.GetUnrealEditor()->GetEditorPanel("RabbitGameUIPanel");
    auto RabbitPanel = std::dynamic_pointer_cast<RabbitGameUIPanel>(Panel);

    RabbitPanel->StartSuccessEffect();

    FSoundManager::GetInstance().StopAllSounds();
    FSoundManager::GetInstance().PlaySound("Success");
  
}

void ARabbitGameMode::Restart()
{
    FTransform SpawnTransform = GetPlayerStartTransform();
    
    if (APlayerController* PlayerController = GEngine->ActiveWorld->GetPlayerController())
    {
        if (ARabbitPlayer* Rabbit = Cast<ARabbitPlayer>(PlayerController->GetPawn()))
        {
            Rabbit->ResetPlayer();
            Rabbit->SetActorLocation(SpawnTransform.GetTranslation());
            CapturedPhotoTypes.Empty();
            Rabbit->GetRabbitCamera()->ResetRabbitCamera(EPhotoTypeSize);
            IsPictureComplete = false;
        }
    }

    if (APlayerController* PlayerController = GEngine->ActiveWorld->GetPlayerController())
    {
        if (ARabbitPlayer* Rabbit = Cast<ARabbitPlayer>(PlayerController->GetPawn()))
        {
            Rabbit->SetIsCaptureAll(false);
        }
    }

    FSoundManager::GetInstance().StopAllSounds();
    FSoundManager::GetInstance().PlaySound("MainBGM");
}
