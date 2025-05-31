#include "RabbitGameMode.h"
#include "Engine/Contents/GameFramework/RabbitPawn.h"
#include "Engine/Contents/GameFramework/RabbitController.h"

ARabbitGameMode::ARabbitGameMode() : AGameMode()
{
    // 기본 플레이어 Pawn 클래스 설정
    DefaultPawnClass = ARabbitPawn::StaticClass();

    // 플레이어 컨트롤러 클래스 설정
    PlayerControllerClass = ARabbitController::StaticClass();
}
