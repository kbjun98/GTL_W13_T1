#include "AnimAttackNotify.h"

void UAnimAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    OnAttackDelegate.Broadcast(MeshComp);
}
