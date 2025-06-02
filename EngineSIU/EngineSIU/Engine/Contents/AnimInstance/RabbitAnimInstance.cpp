#include "RabbitAnimInstance.h"
#include <Engine/AssetManager.h>
#include "Animation/AnimStateMachine.h"
#include "UObject/ObjectFactory.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequence.h"
#include "Engine/SkeletalMesh.h"
#include <Animation/AnimationRuntime.h>
#include <Animation/RabbitAnimStateMachine.h>
#include <GameFramework/RabbitPawn.h>
#include <Engine/Engine.h>

RabbitAnimInstance::RabbitAnimInstance()
    : PrevAnim(nullptr)
    , CurrAnim(nullptr)
    , ElapsedTime(0.f)
    , PlayRate(1.f)
    , bLooping(true)
    , bPlaying(true)
    , bReverse(false)
    , LoopStartFrame(0)
    , LoopEndFrame(0)
    , CurrentKey(0)
    , BlendAlpha(0.f)
    , BlendStartTime(0.f)
    , BlendDuration(0.7f)
    , bIsBlending(false)
{
    StateMachine = FObjectFactory::ConstructObject<RabbitAnimStateMachine>(this);
    Idle = UAssetManager::Get().GetAnimation(FString("Contents/Bunny/Idle"));
    Walk = UAssetManager::Get().GetAnimation(FString("Contents/Bunny/Walk"));
    Attack = UAssetManager::Get().GetAnimation(FString("Contents/Bunny/Attack"));

    CurrAnim = Cast<UAnimSequence>(Idle);
    PrevAnim = Cast<UAnimSequence>(Idle);
}

void RabbitAnimInstance::NativeInitializeAnimation()
{

}


void RabbitAnimInstance::NativeUpdateAnimation(float DeltaSeconds, FPoseContext& OutPose)
{
    UAnimInstance::NativeUpdateAnimation(DeltaSeconds, OutPose);
    USkeletalMeshComponent* SkeletalMeshComp = GetSkelMeshComponent();
    ARabbitPawn* RabbitPawn = Cast<ARabbitPawn>(SkeletalMeshComp->GetOwner());
   StateMachine->ProcessState(GEngine->State);

#pragma region MyAnim

    if (!PrevAnim || !CurrAnim || !SkeletalMeshComp->GetSkeletalMeshAsset() || !SkeletalMeshComp->GetSkeletalMeshAsset()->GetSkeleton() || !bPlaying)
    {
        return;
    }

    ElapsedTime += DeltaSeconds;

    if (bIsBlending)
    {
        float BlendElapsed = ElapsedTime - BlendStartTime;
        BlendAlpha = FMath::Clamp(BlendElapsed / BlendDuration, 0.f, 1.f);

        if (BlendAlpha >= 1.f)
        {
            bIsBlending = false;
        }
    }
    else
    {
        BlendAlpha = 1.f;
    }

    // TODO: FPoseContext의 BoneContainer로 바꾸기
    const FReferenceSkeleton& RefSkeleton = this->GetCurrentSkeleton()->GetReferenceSkeleton();

    if (PrevAnim->GetSkeleton()->GetReferenceSkeleton().GetRawBoneNum() != RefSkeleton.RawRefBoneInfo.Num() || CurrAnim->GetSkeleton()->GetReferenceSkeleton().GetRawBoneNum() != RefSkeleton.RawRefBoneInfo.Num())
    {
        return;
    }

    FPoseContext PrevPose(this);
    FPoseContext CurrPose(this);

    PrevPose.Pose.InitBones(RefSkeleton.RawRefBoneInfo.Num());
    CurrPose.Pose.InitBones(RefSkeleton.RawRefBoneInfo.Num());
    for (int32 BoneIdx = 0; BoneIdx < RefSkeleton.RawRefBoneInfo.Num(); ++BoneIdx)
    {
        PrevPose.Pose[BoneIdx] = RefSkeleton.RawRefBonePose[BoneIdx];
        CurrPose.Pose[BoneIdx] = RefSkeleton.RawRefBonePose[BoneIdx];
    }

    std::cout << GetElapsedTime() << std::endl;

    FAnimExtractContext ExtractA(GetElapsedTime(), false);
    FAnimExtractContext ExtractB(GetElapsedTime(), false);

    PrevAnim->GetAnimationPose(PrevPose, ExtractA);
    CurrAnim->GetAnimationPose(CurrPose, ExtractB);

    FAnimationRuntime::BlendTwoPosesTogether(CurrPose.Pose, PrevPose.Pose, BlendAlpha, OutPose.Pose);
#pragma endregion
}

void RabbitAnimInstance::SetAnimState(EAnimState InAnimState)
{
}

UAnimSequence* RabbitAnimInstance::GetAnimForState(EAnimState InAnimState)
{
    return nullptr;
}
