#include "Pawn.h"
#include "Components/InputComponent.h"
void APawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APawn::PossessedBy(APlayerController* NewController)
{
    Controller = NewController;
}

void APawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    if (PlayerInputComponent)
    {
        PlayerInputComponent->BindAction("W", [this](float DeltaTime) { MoveForward(DeltaTime); });
        PlayerInputComponent->BindAction("S", [this](float DeltaTime) { MoveForward(-DeltaTime); });
        PlayerInputComponent->BindAction("D", [this](float DeltaTime) { MoveRight(DeltaTime); });
        PlayerInputComponent->BindAction("A", [this](float DeltaTime) { MoveRight(-DeltaTime); });
    }
}

void APawn::MoveForward(float DeltaTime)
{
    FVector Delta = FVector(1,0,0) * MoveSpeed * DeltaTime;
    FRotator Rotation = GetActorRotation();
    GetRootComponent()->MoveComponent(Delta, Rotation, false);
}

void APawn::MoveRight(float DeltaTime)
{
    FVector Delta = FVector(0, 1, 0) * MoveSpeed * DeltaTime;
    FRotator Rotation = GetActorRotation();
    GetRootComponent()->MoveComponent(Delta, Rotation, false);
}
