#include "ChaserGameMode.h"
#include "ChasePawn.h"
#include "World/World.h"
#include "ChaserController.h"

void AChaserGameMode::StartMatch()
{
    Super::StartMatch();

    //  추적 AI 생성
    AChasePawn* Chaser = GetWorld()->SpawnActor<AChasePawn>();
    AChaserController* AIController = GetWorld()->SpawnActor<AChaserController>();
    //AIController->Possess(Chaser);

    // 플레이어 우치ㅣ를 추적 대상으로 지정
    APawn* PlayerPawn = GetWorld()->GetMainPlayer();
    if (PlayerPawn)
    {
        AIController->StartChasing(PlayerPawn->GetActorLocation());
    }

}
