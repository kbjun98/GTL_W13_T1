#pragma once

#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"

#include "Container/Array.h"
#include "World/WorldContext.h"
#include <Animation/RabbitAnimStateMachine.h>

class UAssetManager;
class UWorld;
class FPhysicsManager;


class UEngine : public UObject
{
    DECLARE_ABSTRACT_CLASS(UEngine, UObject)

public:
    UEngine() = default;

public:
    UWorld* ActiveWorld;
    ERabbitAnimState State;
    UAssetManager* AssetManager = nullptr;
    FPhysicsManager* PhysicsManager = nullptr;

protected:
    TArray<FWorldContext*> WorldList;
    int32 NextWorldContextHandle = 0;

public:
    virtual void Init();
    virtual void Tick(float DeltaTime) = 0;
    virtual void Release();

    // TODO: UObject->GetWorld() 구현 이후 추가.
    UWorld* GetWorldFromContextObject(const UObject* Object) const;
    FWorldContext* GetWorldContextFromWorld(const UWorld* InWorld);

    FWorldContext& CreateNewWorldContext(EWorldType InWorldType);

    
    void LoadLevel(const FString& FileName) const;
    void SaveLevel(const FString& FileName) const;
};

extern class UEngine* GEngine;
