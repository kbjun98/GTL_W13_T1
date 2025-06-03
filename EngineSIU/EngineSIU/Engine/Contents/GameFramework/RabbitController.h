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

    virtual void Tick(float DeltaTime) override;

    float GetBaseMouseSensitivity() const { return MouseSensitivityBase; }
    float GetMinMouseSensitivity() const { return MouseSensitivityMin; }
    float GetMaxMouseSensitivity() const { return MouseSensitivityMax; }

    float MouseSensitivityCurrent;
    
protected:
    virtual void SetupInputComponent() override;
    
private:
    void MoveForward();
    void MoveBack();
    void MoveRight();
    void MoveLeft();

    void Jump();

    void ZoomIn(float DeltaTime);
    void ZoomOut(float DeltaTime);
    
    virtual void AddYawInput(float Value) override;
    virtual void AddPitchInput(float Value) override;

    void SetInputMode(EInputMode NewInputMode);
    void OnESCPressed();

    void TakePicture();

    void ToggleADS();

    void StartADS();
    void EndADS();

private:
    EInputMode CurrentInputMode = EInputMode::GameOnly;
    FVector2D MousePinPosition;

    float MouseSensitivityBase = 0.4f;
    float MouseSensitivityMin = 0.08f;
    float MouseSensitivityMax = 0.3f;
};

