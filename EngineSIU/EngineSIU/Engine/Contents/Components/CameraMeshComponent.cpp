#include "CameraMeshComponent.h"

#include "Engine/AssetManager.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/RabbitPlayer.h"
#include "UObject/Casts.h"

class ARabbitPlayer;

UCameraMeshComponent::UCameraMeshComponent()
{
    FString MeshName = "Contents/Camera/Camera";
    if (UStaticMesh* Mesh = Cast<UStaticMesh>(UAssetManager::Get().GetAsset(EAssetType::StaticMesh, MeshName)))
    {
        SetStaticMesh(Mesh);
    }

    TargetLocalLocation = FVector(17.f, 5.0f, -12.f);
    TargetLocalRotation = FRotator(0.f, 0.f, 4.f);
    
    SetRelativeLocation(TargetLocalLocation);
    SetRelativeRotation(TargetLocalRotation);
}

void UCameraMeshComponent::TickComponent(float DeltaTime)
{
    Super::TickComponent(DeltaTime);

    // Anim
    Anim(DeltaTime);
    
    // Lerp
    FMatrix CurrentWorldMatrix = GetWorldMatrix();

    FMatrix TargetLocalMatrix = FTransform(TargetLocalRotation, AnimLocalLocation).ToMatrixNoScale();
    SetRelativeTransform(FTransform(TargetLocalMatrix));
    FMatrix TargetWorldMatrix = GetWorldMatrix();

    FVector CurrentLocation = CurrentWorldMatrix.GetTranslationVector();
    FVector TargetLocation = TargetWorldMatrix.GetTranslationVector();
    FVector InterpLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, InterpSpeed);

    FQuat CurrentRotation = CurrentWorldMatrix.ToQuat();
    FQuat TargetRotation = TargetWorldMatrix.ToQuat();
    FQuat InterpRotation = FMath::QInterpTo(CurrentRotation, TargetRotation, DeltaTime, InterpSpeed);

    // Set
    SetWorldTransform(FTransform(InterpRotation, InterpLocation));
}

void UCameraMeshComponent::Anim(float DeltaTime)
{
    AnimLocalLocation = TargetLocalLocation;
    
    if (ARabbitPlayer* RabbitPlayer = Cast<ARabbitPlayer>(GetOwner()))
    {
        if (UPawnMovementComponent* MoveComp = RabbitPlayer->GetMovementComponent())
        {
            FVector VelocityXY = FVector(MoveComp->Velocity.X, MoveComp->Velocity.Y, 0.f);
            if (VelocityXY.SquaredLength() < 10000.f)
            {
                bIsMoving = false;
            }
            else
            {
                if (!bIsMoving)
                {
                    bIsMoving = true;
                    MovingTime = 0.f;
                }
            }
        }
    }

    if (bIsMoving)
    {
        MovingTime += DeltaTime;
        AnimLocalLocation.Z = TargetLocalLocation.Z + FMath::Sin(MovingTime * 10.f) * 5.f;
        AnimLocalLocation.Y = TargetLocalLocation.Y + FMath::Sin(MovingTime * 5.f - 0.5f) * 3.f;
    }
}
