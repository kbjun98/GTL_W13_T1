#pragma once
#include "GameFramework/PlayerController.h"

enum class EInputMode : uint8
{
    UIOnly,
    GameOnly,
    GameAndUI
};


class ARabbitController : public APlayerController
{
    DECLARE_CLASS(ARabbitController, APlayerController)
public:
    ARabbitController() = default;
    virtual ~ARabbitController() override = default;

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
protected:
    virtual void SetupInputComponent() override;
private:
    void MoveForward(float DeltaTime);
    void MoveRight(float DeltaTime);
    void RotateYaw(float DeltaTime);
    void RotatePitch(float DeltaTime);

    void SetInputMode(EInputMode NewInputMode);
    void OnESCPressed();
private:
    EInputMode CurrentInputMode = EInputMode::GameOnly;
    FVector2D MousePinPosition;
};

