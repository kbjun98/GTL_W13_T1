#pragma once
#include "Actor.h"
#include "Delegates/DelegateCombination.h"
#include "Template/SubclassOf.h"

DECLARE_MULTICAST_DELEGATE(FOnGameInit);
DECLARE_MULTICAST_DELEGATE(FOnGameStart);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameEnd, bool);

class UCameraComponent;
class APlayerController;
class APlayer;
class APawn;

struct FGameInfo
{
    float TotalGameTime = 0.0f;
    float ElapsedGameTime = 0.0f;
};

class AGameMode : public AActor
{
    DECLARE_CLASS(AGameMode, AActor)
    
public:
    AGameMode();
    virtual ~AGameMode() override = default;

    virtual void PostSpawnInitialize() override;
    
    virtual UObject* Duplicate(UObject* InOuter) override;

    //virtual void BeginPlay() override;
    //virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // 게임 모드 초기화
    virtual void InitGame();

    // 게임 시작
    virtual void StartMatch();

    // 게임 종료
    virtual void EndMatch(bool bIsWin);

    virtual void Tick(float DeltaTime) override;

    void Reset();

    void HandleStartingNewPlayer();
    APlayerController* SpawnPlayerController();
    APawn* SpawnDefaultPlayer();

    FOnGameInit OnGameInit;
    FOnGameStart OnGameStart;
    FOnGameEnd OnGameEnd;

    FGameInfo GameInfo;

    TSubclassOf<APawn> DefaultPawnClass; // 기본 플레이어 Pawn 클래스
    
    TSubclassOf<APlayerController> PlayerControllerClass; // 플레이어 컨트롤러 클래스
    
private:
    bool bGameRunning; // 내부 
    bool bGameEnded;

    float LogTimer = 0.f;
    float LogInterval = 1.f;  // 1초마다 로그
};

