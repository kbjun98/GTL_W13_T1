#pragma once
#include "Pawn.h"

class UCapsuleComponent;
class USkeletalMeshComponent;

class ACharacter : public APawn
{
    DECLARE_CLASS(ACharacter, APawn)

public:
    ACharacter() = default;

    UCapsuleComponent* Capsule = nullptr;
    //MovementComponent* Movement;
    USkeletalMeshComponent* Mesh = nullptr;
};
