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

    SetRelativeLocation(FVector(17.f, 5.0f, -12.f));
    SetRelativeRotation(FRotator(0.f, 0.f, 4.f));
}

void UCameraMeshComponent::TickComponent(float DeltaTime)
{
    Super::TickComponent(DeltaTime);

    
}
