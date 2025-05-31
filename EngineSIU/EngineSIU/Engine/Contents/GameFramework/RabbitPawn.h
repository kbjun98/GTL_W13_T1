#pragma once
#include "GameFramework/Pawn.h"

class ARabbitPawn : public APawn
{
    DECLARE_CLASS(ARabbitPawn, APawn)
public:
    ARabbitPawn() = default;
    virtual void Tick(float DeltaTime) override;
    virtual void PostSpawnInitialize() override;
public:
    void MoveForward(float DeltaTime);
    void MoveRight(float DeltaTime);
    void RotateYaw(float DeltaTime);
    void RotatePitch(float DeltaTime);

    std::shared_ptr<PlayerCamera> GetPlayerCamera();

private:
    float MoveSpeed = 100.0f; // 이동 속도  
    float RotateSpeed = 100.0f; // 회전 속도
    std::shared_ptr<PlayerCamera> PlayerCam;
 
};

