#include "RabbitGameMode.h"
#include "Engine/Contents/GameFramework/RabbitPawn.h"
#include "Engine/Contents/GameFramework/RabbitController.h"
#include <Engine/Engine.h>
#include "World/World.h"

ARabbitGameMode::ARabbitGameMode() : AGameMode()
{
    // 기본 플레이어 Pawn 클래스 설정
    DefaultPawnClass = ARabbitPawn::StaticClass();

    // 플레이어 컨트롤러 클래스 설정
    PlayerControllerClass = ARabbitController::StaticClass();
}

void ARabbitGameMode::BeginPlay()
{
    Super::BeginPlay();
 
    if (APlayerController* PlayerContoller = GEngine->ActiveWorld->GetPlayerController())
    {
        if (ARabbitPawn* Rabbit = Cast<ARabbitPawn>(PlayerContoller->GetPawn()))
        {
            if (std::shared_ptr<RabbitCamera> Camera = Rabbit->GetPlayerCamera())
            {
                Camera->OnPictureTaken.BindLambda([&](UPrimitiveComponent* Comp) 
                    {
                        JudgeCapturedPhoto(Comp);
                    });
            }
        }

    }
    
}

void ARabbitGameMode::JudgeCapturedPhoto(UPrimitiveComponent* CapturedComp)
{
    std::cout << "찍힘" << CapturedComp;
}
