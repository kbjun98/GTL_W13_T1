#include "RabbitController.h"
#include "Engine/Contents/GameFramework/RabbitPawn.h"
#include "UObject/Casts.h"
#include "Windows/WindowsCursor.h"

void ARabbitController::BeginPlay()
{
    Super::BeginPlay();
    SetInputMode(EInputMode::GameOnly);
}

void ARabbitController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    SetInputMode(EInputMode::GameAndUI); // 게임 종료시 UI 모드로 전환
}

void ARabbitController::SetupInputComponent()
{

    Super::SetupInputComponent();
    InputComponent->BindAction("W", [this](float DeltaTime) { MoveForward(DeltaTime); });
    InputComponent->BindAction("S", [this](float DeltaTime) { MoveForward(-DeltaTime); });
    InputComponent->BindAction("D", [this](float DeltaTime) { MoveRight(DeltaTime); });
    InputComponent->BindAction("A", [this](float DeltaTime) { MoveRight(-DeltaTime); });

    InputComponent->BindAxis("Turn", [this](float DeltaTime) { RotateYaw(DeltaTime); });
    InputComponent->BindAxis("LookUp", [this](float DeltaTime) { RotatePitch(DeltaTime); });

    InputComponent->BindAction("ESC_Pressed", [this](float DeltaTime) { OnESCPressed(); });

    //마우스 클릭
    InputComponent->BindAction("L_Pressed", [this](float DeltaTime) {TakePicture();});
    //InputComponent->BindAction("R_Pressed", [this](float DeltaTime) {
    //    UE_LOG(ELogLevel::Display, TEXT("Right Mouse Button Pressed"));
    //    });
    //InputComponent->BindAction("L_Released", [this](float DeltaTime) {
    //    UE_LOG(ELogLevel::Display, TEXT("Left Mouse Button Released"));
    //    });
    //InputComponent->BindAction("R_Released", [this](float DeltaTime) {
    //    UE_LOG(ELogLevel::Display, TEXT("Right Mouse Button Released"));
    //    });
    //InputComponent->BindAction("L_Pressing", [this](float DeltaTime) {
    //    UE_LOG(ELogLevel::Display, TEXT("Left Mouse Button Pressing"));
    //    });
    //InputComponent->BindAction("R_Pressing", [this](float DeltaTime) {
    //    UE_LOG(ELogLevel::Display, TEXT("Right Mouse Button Pressing"));
    //    });
}

void ARabbitController::MoveForward(float DeltaTime)
{
    if (CurrentInputMode == EInputMode::UIOnly) return;

    if (ARabbitPawn* RabbitPawn = Cast<ARabbitPawn>(PossessedPawn))
    {
        RabbitPawn->MoveForward(DeltaTime);
    }
}

void ARabbitController::MoveRight(float DeltaTime)
{
    if (CurrentInputMode == EInputMode::UIOnly) return;

    if (ARabbitPawn* RabbitPawn = Cast<ARabbitPawn>(PossessedPawn))
    {
        RabbitPawn->MoveRight(DeltaTime);
    }
}

void ARabbitController::RotateYaw(float DeltaTime)
{
    if (CurrentInputMode == EInputMode::UIOnly) return;

    if (ARabbitPawn* RabbitPawn = Cast<ARabbitPawn>(PossessedPawn))
    {
        RabbitPawn->RotateYaw(DeltaTime);
    }
}

void ARabbitController::RotatePitch(float DeltaTime)
{
    if (CurrentInputMode == EInputMode::UIOnly) return;

    if (ARabbitPawn* RabbitPawn = Cast<ARabbitPawn>(PossessedPawn))
    {
        RabbitPawn->RotatePitch(DeltaTime);
    }
}

void ARabbitController::SetInputMode(EInputMode NewInputMode)
{
    CurrentInputMode = NewInputMode;
    switch (CurrentInputMode)
    {
    case EInputMode::UIOnly:
        FWindowsCursor::SetShowMouseCursor(true);
        FWindowsCursor::LockToWindow(false);
        FWindowsCursor::SetPosition(MousePinPosition.X, MousePinPosition.Y);
        break;
    case EInputMode::GameOnly:
        FWindowsCursor::SetShowMouseCursor(false);
        FWindowsCursor::LockToWindow(true);
        MousePinPosition = FWindowsCursor::GetPosition();
        break;
    case EInputMode::GameAndUI:
        FWindowsCursor::SetShowMouseCursor(true);
        FWindowsCursor::LockToWindow(false);
        break;
    }
}

void ARabbitController::OnESCPressed()
{
    if (CurrentInputMode == EInputMode::UIOnly)
    {
        SetInputMode(EInputMode::GameOnly);
    }
    else
    {
        SetInputMode(EInputMode::UIOnly);
    }
}

void ARabbitController::TakePicture()
{
    if (CurrentInputMode == EInputMode::UIOnly) return;

    if (ARabbitPawn* RabbitPawn = Cast<ARabbitPawn>(PossessedPawn))
    {
        RabbitPawn->GetPlayerCamera()->TakePicture();
    }
}
