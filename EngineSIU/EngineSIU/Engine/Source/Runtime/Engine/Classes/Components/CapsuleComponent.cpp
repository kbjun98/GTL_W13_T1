#include "CapsuleComponent.h"

#include "PhysicsManager.h"
#include "Engine/Engine.h"
#include "UObject/Casts.h"

UCapsuleComponent::UCapsuleComponent()
{
    ShapeType = EShapeType::Capsule;
}

UObject* UCapsuleComponent::Duplicate(UObject* InOuter)
{
    ThisClass* NewComponent = Cast<ThisClass>(Super::Duplicate(InOuter));

    NewComponent->CapsuleHalfHeight = CapsuleHalfHeight;
    NewComponent->CapsuleRadius = CapsuleRadius;
    
    return NewComponent;
}

void UCapsuleComponent::SetProperties(const TMap<FString, FString>& InProperties)
{
    Super::SetProperties(InProperties);
    const FString* TempStr = nullptr;
    TempStr = InProperties.Find(TEXT("CapsuleHalfHeight"));
    if (TempStr)
    {
        CapsuleHalfHeight = FCString::Atof(**TempStr);
    }
    TempStr = InProperties.Find(TEXT("CapsuleRadius"));
    if (TempStr)
    {
        CapsuleRadius = FCString::Atof(**TempStr);
    }
}

void UCapsuleComponent::GetProperties(TMap<FString, FString>& OutProperties) const
{
    Super::GetProperties(OutProperties);
    OutProperties.Add(TEXT("CapsuleHalfHeight"), FString::SanitizeFloat(CapsuleHalfHeight));
    OutProperties.Add(TEXT("CapsuleRadius"), FString::SanitizeFloat(CapsuleRadius));
}

void UCapsuleComponent::GetEndPoints(FVector& OutStart, FVector& OutEnd) const
{
    const float LineHalfLength = CapsuleHalfHeight - CapsuleRadius;
    OutStart = GetComponentLocation() + GetUpVector() * LineHalfLength;
    OutEnd = GetComponentLocation() - GetUpVector() * LineHalfLength;
}

void UCapsuleComponent::CreatePhysXGameObject()
{
    if (!bSimulate)
    {
        return;
    }
    
    BodyInstance = new FBodyInstance(this);

    BodyInstance->bSimulatePhysics = bSimulate;
    BodyInstance->bEnableGravity = bApplyGravity;
    
    FVector Location = GetComponentLocation();
    PxVec3 PPos = PxVec3(Location.X, Location.Y, Location.Z);
    
    FQuat Quat = GetComponentRotation().Quaternion();
    PxQuat PQuat = PxQuat(Quat.X, Quat.Y, Quat.Z, Quat.W);

    if (GeomAttributes.Num() == 0)
    {
        PxVec3 RelativePos = PxVec3(0.f, 0.f, 0.f);
        PxQuat RelativeQuat = PxQuat(0.f, 0.f, 0.f, 1.f);
        PxShape* CapsuleShape = GEngine->PhysicsManager->CreateCapsuleShape(RelativePos, RelativeQuat, CapsuleRadius, CapsuleHalfHeight);
        BodySetup->AggGeom.CapsuleElems.Add(CapsuleShape);
    }
    
    GameObject* Obj = GEngine->PhysicsManager->CreateGameObject(PPos, PQuat, BodyInstance,  BodySetup, RigidBodyType);
}
