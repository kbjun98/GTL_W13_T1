#include "SphereComponent.h"

#include "PhysicsManager.h"
#include "Engine/Engine.h"
#include "UObject/Casts.h"

USphereComponent::USphereComponent()
{
    ShapeType = EShapeType::Sphere;
}

UObject* USphereComponent::Duplicate(UObject* InOuter)
{
    ThisClass* NewComponent = Cast<ThisClass>(Super::Duplicate(InOuter));

    NewComponent->SphereRadius = SphereRadius;

    return NewComponent;
}

void USphereComponent::SetProperties(const TMap<FString, FString>& InProperties)
{
    Super::SetProperties(InProperties);
    const FString* TempStr = nullptr;
    TempStr = InProperties.Find(TEXT("SphereRadius"));
    if (TempStr)
    {
        SphereRadius = FCString::Atof(**TempStr);
    }
}

void USphereComponent::GetProperties(TMap<FString, FString>& OutProperties) const
{
    Super::GetProperties(OutProperties);
    OutProperties.Add(TEXT("SphereRadius"), FString::SanitizeFloat(SphereRadius));
}

void USphereComponent::CreatePhysXGameObject()
{
    if (!bSimulate)
    {
        return;
    }
    
    BodyInstance = new FBodyInstance(this);

    ////////////// 테스트 코드
    BodyInstance->bSimulatePhysics = bSimulate;
    BodyInstance->bEnableGravity = bApplyGravity;
    ////////////////////////
    
    FVector Location = GetComponentLocation();
    PxVec3 PPos = PxVec3(Location.X, Location.Y, Location.Z);
    
    FQuat Quat = GetComponentRotation().Quaternion();
    PxQuat PQuat = PxQuat(Quat.X, Quat.Y, Quat.Z, Quat.W);

    if (GeomAttributes.Num() == 0)
    {
        AggregateGeomAttributes DefaultAttribute;
        DefaultAttribute.GeomType = EGeomType::ESphere;
        DefaultAttribute.Offset = FVector::ZeroVector;
        DefaultAttribute.Extent = FVector(AABB.MaxLocation - AABB.MinLocation) / 2 * GetComponentScale3D();
        GeomAttributes.Add(DefaultAttribute);

        PxVec3 Offset = PxVec3(0.f, 0.f, 0.f);
        PxQuat GeomPQuat = PxQuat(0.f, 0.f, 0.f, 1.f);
        PxShape* PxSphere = GEngine->PhysicsManager->CreateSphereShape(Offset, GeomPQuat, GetRadius());
        BodySetup->AggGeom.SphereElems.Add(PxSphere);
    }
    
    GameObject* Obj = GEngine->PhysicsManager->CreateGameObject(PPos, PQuat, BodyInstance,  BodySetup, RigidBodyType);
}
