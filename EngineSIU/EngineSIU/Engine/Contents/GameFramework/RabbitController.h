#pragma once
#include "GameFramework/PlayerController.h"
class ARabbitController : public APlayerController
{
    DECLARE_CLASS(ARabbitController, APlayerController)
public:
    ARabbitController() = default;
    virtual ~ARabbitController() override = default;
protected:
    virtual void SetupInputComponent() override;
private:
    void MoveForward(float DeltaTime);
    void MoveRight(float DeltaTime);
};

