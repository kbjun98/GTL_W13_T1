#pragma once
#include "RabbitPawn.h"
#include "UObject/ObjectMacros.h"

class ARabbitPlayer : public ARabbitPawn
{
    DECLARE_CLASS(ARabbitPlayer, ARabbitPawn)
    
public:
    ARabbitPlayer() = default;
    
    virtual void PostSpawnInitialize() override;
    
    virtual void Tick(float DeltaTime) override;
    
    virtual UObject* Duplicate(UObject* InOuter) override;

    std::shared_ptr<RabbitCamera> GetRabbitCamera();

    virtual FVector GetActorForwardVector() const override;
    virtual FVector GetActorRightVector() const override;

    void Jump();
private:

    std::shared_ptr<RabbitCamera> RabbitCam = nullptr;
};

