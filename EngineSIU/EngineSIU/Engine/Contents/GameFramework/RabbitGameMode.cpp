#include "RabbitGameMode.h"
#include "Engine/Contents/GameFramework/RabbitPawn.h"
#include "Engine/Contents/GameFramework/RabbitPlayer.h"
#include "Engine/Contents/GameFramework/RabbitController.h"
#include <Engine/Engine.h>
#include "World/World.h"

ARabbitGameMode::ARabbitGameMode()
{
    // 기본 플레이어 Pawn 클래스 설정
    DefaultPawnClass = ARabbitPlayer::StaticClass();

    // 플레이어 컨트롤러 클래스 설정
    PlayerControllerClass = ARabbitController::StaticClass();

    EPhotoTypeSize = static_cast<int>(EPhotoType::END) - 1;
}

void ARabbitGameMode::BeginPlay()
{
    Super::BeginPlay();
 
    if (APlayerController* PlayerController = GEngine->ActiveWorld->GetPlayerController())
    {
        if (ARabbitPlayer* Rabbit = Cast<ARabbitPlayer>(PlayerController->GetPawn()))
        {
            if (std::shared_ptr<RabbitCamera> Camera = Rabbit->GetRabbitCamera())
            {
                Camera->InitPictureArraySize(EPhotoTypeSize);

                Camera->OnPictureTaken.BindLambda(
                    [this](UPrimitiveComponent* Comp,RabbitCamera* RabbitCam) 
                    {
                        this->JudgeCapturedPhoto(Comp,RabbitCam);
                    }
                );
            }
        }
    }
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

    if (CapturedPhotoTypes.Num() == EPhotoTypeSize)
    {
        std::cout << "다찍었다 드가자~";
    }
   
}
