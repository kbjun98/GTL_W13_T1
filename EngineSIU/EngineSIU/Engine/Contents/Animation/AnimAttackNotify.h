#pragma once
#include "Animation/AnimNotify.h"
#include "Delegates/DelegateCombination.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAttackDelegate, USkeletalMeshComponent*)

class UAnimAttackNotify : public UAnimNotify
{
    DECLARE_CLASS(UAnimAttackNotify, UAnimNotify)
public:
    UAnimAttackNotify() = default;
    virtual FName GetNotifyName() const override { return FName("AnimAttackNotify"); }
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
    FOnAttackDelegate OnAttackDelegate;
};

