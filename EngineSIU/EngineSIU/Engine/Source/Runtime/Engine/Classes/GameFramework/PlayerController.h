#pragma once
#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/Actor.h" 
#include "Classes/Components/InputComponent.h"
#include "Classes/GameFramework/Controller.h"

class APlayerCameraManager;
class APawn;

class APlayerController : public AController
{
    DECLARE_CLASS(APlayerController, AActor)
    
public:
    APlayerController() = default;
    virtual ~APlayerController() override = default;

    virtual void PostSpawnInitialize() override;

    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;
    
    void ProcessInput(float DeltaTime) const;

    virtual void Destroyed() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UInputComponent* GetInputComponent() const { return InputComponent; }

    void SetViewTarget(class AActor* NewViewTarget, struct FViewTargetTransitionParams TransitionParams);

    virtual void Possess(APawn* InPawn);

    virtual void UnPossess();
    
    virtual void BindAction(const FString& Key, const std::function<void(float)>& Callback);

    APawn* GetPawn() const { return PossessedPawn; }
    
    // 카메라 관련 함수
    AActor* GetViewTarget() const;

    virtual void SpawnPlayerCameraManager();

    void ClientStartCameraShake(UClass* Shake);

    void ClientStopCameraShake(UClass* Shake, bool bImmediately = true);

    APlayerCameraManager* PlayerCameraManager = nullptr;

    virtual void AddPitchInput(float Value);
    virtual void AddYawInput(float Value);

    FRotator GetControlRotation() const { return ControlRotation; }
    
protected:
    UPROPERTY(UInputComponent*, InputComponent, = nullptr)

    virtual void SetupInputComponent();

    APawn* PossessedPawn = nullptr;

    bool bHasPossessed = false;

    FRotator RotationInput = FRotator(0, 0, 0);

    FRotator ControlRotation = FRotator(0, 0, 0);
};

