#pragma once
#include "RabbitPawn.h"
#include "Template/SubclassOf.h"
#include "UObject/ObjectMacros.h"

class UCameraShakeBase;

class ARabbitPlayer : public ARabbitPawn
{
    DECLARE_CLASS(ARabbitPlayer, ARabbitPawn)
    
public:
    ARabbitPlayer() = default;
    
    virtual void PostSpawnInitialize() override;

    virtual void BeginPlay() override;
    
    virtual void Tick(float DeltaTime) override;
    
    virtual UObject* Duplicate(UObject* InOuter) override;

    std::shared_ptr<RabbitCamera> GetRabbitCamera();

    virtual FVector GetActorForwardVector() const override;
    virtual FVector GetActorRightVector() const override;

    void Jump();
    
    bool IsADS() const { return bIsADS; }

    void TakePicture();
    
    void ToggleADS();

protected:
    void StartADS();
    void EndADS();
    
private:
    std::shared_ptr<RabbitCamera> RabbitCam = nullptr;
    
    bool bIsADS = false;

    float DefaultADSFOV = 50.f;
    float DefaultFOV = 100.f;

    TSubclassOf<UCameraShakeBase> IdleCameraShake = UClass::FindClass("UIdleCameraShake");

    UCameraShakeBase* CameraShakeInstance = nullptr;
};

