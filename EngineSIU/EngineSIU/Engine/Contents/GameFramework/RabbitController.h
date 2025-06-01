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
    void MoveForward();
    void MoveBack();
    void MoveRight();
    void MoveLeft();

    void Jump();
    
    virtual void AddYawInput(float Value) override;
    virtual void AddPitchInput(float Value) override;

    void SetInputMode(EInputMode NewInputMode);
    void OnESCPressed();

    void TakePicture();

private:
    EInputMode CurrentInputMode = EInputMode::GameOnly;
    FVector2D MousePinPosition;

    float MouseSensitivity = 0.4f;
};

