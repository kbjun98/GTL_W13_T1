#include "CameraMeshComponent.h"

#include "Engine/AssetManager.h"
#include "UObject/Casts.h"

UCameraMeshComponent::UCameraMeshComponent()
{
    FString MeshName = "Contents/Camera/Camera";
    if (UStaticMesh* Mesh = Cast<UStaticMesh>(UAssetManager::Get().GetAsset(EAssetType::StaticMesh, MeshName)))
    {
        SetStaticMesh(Mesh);
    }

    FVector Loc = FVector(17.f, 5.0f, -12.f);
    FRotator Rot = FRotator(0.f, 0.f, 4.f);
    
    SetRelativeLocation(Loc);
    SetRelativeRotation(Rot);

    TargetLocalMatrix = FTransform(Rot, Loc).ToMatrixNoScale();
}

void UCameraMeshComponent::TickComponent(float DeltaTime)
{
    Super::TickComponent(DeltaTime);

    FMatrix CurrentWorldMatrix = GetWorldMatrix();
    
    SetRelativeTransform(FTransform(TargetLocalMatrix));
    FMatrix TargetWorldMatrix = GetWorldMatrix();

    FVector CurrentLocation = CurrentWorldMatrix.GetTranslationVector();
    FVector TargetLocation = TargetWorldMatrix.GetTranslationVector();
    FVector InterpLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, InterpSpeed);

    FQuat CurrentRotation = CurrentWorldMatrix.ToQuat();
    FQuat TargetRotation = TargetWorldMatrix.ToQuat();
    FQuat InterpRotation = FMath::QInterpTo(CurrentRotation, TargetRotation, DeltaTime, InterpSpeed);
    
    SetWorldTransform(FTransform(InterpRotation, InterpLocation));
}
