#include "MovementComponent.h"
#include "GameFramework/Actor.h"

UMovementComponent::UMovementComponent()
    : UpdatedComponent(nullptr)
    , MaxSpeed(300.f)
{
}

void UMovementComponent::InitializeComponent()
{
    Super::InitializeComponent();

    if (AActor* MyActor = GetOwner())
    {
        if (USceneComponent* NewUpdatedComponent = MyActor->GetRootComponent())
        {
            SetUpdatedComponent(NewUpdatedComponent);
        }
    }
}

UObject* UMovementComponent::Duplicate(UObject* InOuter)
{
    ThisClass* NewComponent = Cast<ThisClass>(Super::Duplicate(InOuter));

    return NewComponent;
}

void UMovementComponent::BeginPlay()
{
    Super::BeginPlay();

    if (UpdatedComponent == nullptr)
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

void UMovementComponent::TickComponent(float DeltaTime)
{
    Super::TickComponent(DeltaTime);
}

void UMovementComponent::SetUpdatedComponent(USceneComponent* NewUpdatedComponent)
{
    if (NewUpdatedComponent)
    {
        UpdatedComponent = NewUpdatedComponent;
    }
}

bool UMovementComponent::MoveUpdatedComponent(const FVector& Delta, const FQuat& NewRotation)
{
    return MoveUpdatedComponentImpl(Delta, NewRotation);
}

bool UMovementComponent::MoveUpdatedComponent(const FVector& Delta, const FRotator& NewRotation)
{
    return MoveUpdatedComponentImpl(Delta, NewRotation.Quaternion());
}

bool UMovementComponent::MoveUpdatedComponentImpl(const FVector& Delta, const FQuat& NewRotation)
{
    if (UpdatedComponent)
    {
        return UpdatedComponent->MoveComponent(Delta, NewRotation, false, nullptr);
    }

    return false;
}
