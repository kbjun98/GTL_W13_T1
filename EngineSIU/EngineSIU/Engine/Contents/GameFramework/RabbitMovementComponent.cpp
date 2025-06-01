#include "RabbitMovementComponent.h"

#include "PhysicsManager.h"

#include "Components/CapsuleComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/Pawn.h"
#include "UObject/Casts.h"

URabbitMovementComponent::URabbitMovementComponent()
    : bGravity(true)
    , GravityFactor(981.f)
{}


void URabbitMovementComponent::BeginPlay()
{
    Super::BeginPlay();

    if (UpdatedComponent == nullptr || Controller == nullptr)
    {
        if (AActor* MyActor = GetOwner())
        {
            if (USceneComponent* NewUpdatedComponent = MyActor->GetRootComponent())
            {
                SetUpdatedComponent(NewUpdatedComponent);
            }
        }
    }
}

void URabbitMovementComponent::SetUpdatedComponent(USceneComponent* NewUpdatedComponent)
{
    Super::SetUpdatedComponent(NewUpdatedComponent);

    if (NewUpdatedComponent)
    {
        if (UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(NewUpdatedComponent))
        {
            const FVector Location = Capsule->GetComponentLocation();
            const float PxHalfHeight = Capsule->GetHalfHeight() - Capsule->GetRadius();
            const float PxHeight = PxHalfHeight * 2.0f;
            PxMaterial* MyMaterial = GEngine->PhysicsManager->GetPhysics()->createMaterial(0.0f, 0.0f, 0.0f);
            
            PxCapsuleControllerDesc Desc;
            Desc.radius = Capsule->GetRadius(); 
            Desc.height = PxHeight;
            Desc.position = PxExtendedVec3(Location.X, Location.Y, Location.Z - Capsule->GetHalfHeight());
            Desc.material = MyMaterial;
            Desc.stepOffset = 30.0f;
            Desc.slopeLimit = FMath::Cos(FMath::DegreesToRadians(45.0f));
            Desc.contactOffset = 10.0f;
            Desc.climbingMode = PxCapsuleClimbingMode::eEASY;
            Desc.upDirection = PxVec3(0.0f, 0.0f, 1.0f);
            Desc.maxJumpHeight = 0.0f;

            Controller = GEngine->PhysicsManager->CreateCapsuleController(Desc);
            if (!Controller)
            {
                UE_LOG(ELogLevel::Error, TEXT("Failed to create capsule controller"));
            }
        }
    }
}

void URabbitMovementComponent::DestroyComponent(bool bPromoteChildren)
{
    if (Controller)
    {
        Controller->release();
    }
    Super::DestroyComponent(bPromoteChildren);
}

void URabbitMovementComponent::TickComponent(float DeltaTime)
{
    Super::TickComponent(DeltaTime);

    PerformMovement(DeltaTime);
}

void URabbitMovementComponent::PerformMovement(float DeltaTime)
{
    if (UpdatedComponent == nullptr)
    {
        return;
    }

    const FVector PrevVelocity = Velocity;
    Velocity.X = 0.f;
    Velocity.Y = 0.f;

    if (APawn* Pawn = Cast<APawn>(UpdatedComponent->GetOwner()))
    {
        FVector MoveInput = Pawn->ConsumeMovementInputVector().GetSafeNormal();
        Velocity += MoveInput * MaxSpeed;
    }

    if (!bIsGrounded && bGravity)
    {
        Velocity += FVector(0.0f, 0.0f, -GravityFactor * DeltaTime);
    }
    else
    {
        Velocity.Z = -1.f; // 기본적으로 아래로 약간 떨어지게 해서 바닥과의 충돌 판정을 제대로 받게 함.
    }
    
    if (Controller)
    {
        FVector DeltaLocation = Velocity * DeltaTime;
        PxVec3 disp = PxVec3(DeltaLocation.X, DeltaLocation.Y, DeltaLocation.Z);
        PxControllerFilters filters;
        
        PxControllerCollisionFlags Flags = Controller->move(disp, 0.f, DeltaTime, filters);
        bIsGrounded = Flags & PxControllerCollisionFlag::eCOLLISION_DOWN;
        
        const PxExtendedVec3& Pos = Controller->getPosition();
        FVector FinalLocation = FVector(Pos.x, Pos.y, Pos.z);
        UpdatedComponent->SetWorldLocation(FinalLocation);
    }
    else
    {
        MoveUpdatedComponent(Velocity * DeltaTime, GetOwner()->GetActorRotation());
    }
}

void URabbitMovementComponent::Jump()
{
    if (bIsGrounded)
    {
        Velocity.Z = JumpZVelocity;
        bIsGrounded = false;
    }
}
