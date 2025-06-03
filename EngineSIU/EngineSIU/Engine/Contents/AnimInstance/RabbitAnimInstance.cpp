#include "Animation/AnimSequenceBase.h"
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
#include "Animation/AnimData/AnimDataModel.h"
#include "Animation/AnimTypes.h"
#include <Animation/AnimSoundNotify.h>
#include "Animation/AnimAttackNotify.h"
#include "GameFramework/RabbitEnemy.h"



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
    , BlendDuration(1.f)
    , bIsBlending(false)
{
    PrevAnimTimeSnapshot = 0.f;
    StateMachine = FObjectFactory::ConstructObject<RabbitAnimStateMachine>(this);
    Idle = UAssetManager::Get().GetAnimation(FString("Contents/Bunny/Idle"));
    Walk = UAssetManager::Get().GetAnimation(FString("Contents/Bunny/Walk"));
    Attack = UAssetManager::Get().GetAnimation(FString("Contents/Bunny/Attack"));

    CurrAnim = Cast<UAnimSequence>(Idle);
    PrevAnim = Cast<UAnimSequence>(Idle);
}

void RabbitAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();
    AddSoundNotify();
    //AddAttackNotify();
}

float GetNormalizedAnimTime(UAnimSequence* Anim, float ElapsedTime)
{
    if (!Anim) return 0.f;

    float AnimLength = Anim->GetPlayLength();
    if (AnimLength <= 0.f) return 0.f;

    // 루프를 고려한 정규화된 시간
    return FMath::Fmod(ElapsedTime, AnimLength);
}

void RabbitAnimInstance::NativeUpdateAnimation(float DeltaSeconds, FPoseContext& OutPose)
{
    UAnimInstance::NativeUpdateAnimation(DeltaSeconds, OutPose);
    USkeletalMeshComponent* SkeletalMeshComp = GetSkelMeshComponent();
    ARabbitPawn* RabbitPawn = Cast<ARabbitPawn>(SkeletalMeshComp->GetOwner());
   StateMachine->ProcessState(RabbitPawn->GetAnimState());

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


    // 애니메이션 길이 가져오기
    float AnimLength = CurrAnim->GetPlayLength()/30 / 30;

    // 현재 시간을 애니메이션 길이로 정규화
    float NormalizedTime = FMath::Fmod(ElapsedTime, AnimLength);

    // 이전 시간도 정규화 (루프 경계 처리를 위해)
    float NormalizedPreviousTime = FMath::Fmod(PreviousTime, AnimLength);

    // 노티파이 평가
    CurrAnim->EvaluateAnimNotifies(CurrAnim->Notifies, NormalizedTime, NormalizedPreviousTime, DeltaSeconds, SkeletalMeshComp, CurrAnim, bLooping);

    // 이전 시간 업데이트
    PreviousTime = ElapsedTime;

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

    
    if (bIsBlending)
    {
        float BlendElapsed = ElapsedTime - BlendStartTime;
        float CurrAnimLength = CurrAnim->GetPlayLength();
        float CurrAnimTime = FMath::Fmod(BlendElapsed, CurrAnimLength);

        FAnimExtractContext ExtractA(PrevAnimTimeSnapshot, false); // 저장된 시간 사용
        FAnimExtractContext ExtractB(CurrAnimTime, false);

        PrevAnim->GetAnimationPose(PrevPose, ExtractA);
        CurrAnim->GetAnimationPose(CurrPose, ExtractB);
    }
    else
    {
        float CurrAnimLength = CurrAnim->GetPlayLength();
        float CurrAnimTime = FMath::Fmod(ElapsedTime, CurrAnimLength);

        FAnimExtractContext ExtractA(CurrAnimTime, false);
        FAnimExtractContext ExtractB(CurrAnimTime, false);
        PrevAnim->GetAnimationPose(PrevPose, ExtractA);
        CurrAnim->GetAnimationPose(CurrPose, ExtractB);
    }


    FAnimationRuntime::BlendTwoPosesTogether(CurrPose.Pose, PrevPose.Pose, BlendAlpha, OutPose.Pose);
#pragma endregion
}

void RabbitAnimInstance::AddSoundNotify()
{
    int32 AttackTrackIndex;
    bool bTrackAdded = Cast<UAnimSequence>(Attack)->AddNotifyTrack(FName("AttackTrack"), AttackTrackIndex);

    UAnimSequence* AnimSequence = Cast<UAnimSequence>(Attack);

    if (bTrackAdded)
    {
        // 공격 시작 Notify 추가
        int32 AttackStartIndex;
        AnimSequence->AddNotifyEvent(
            AttackTrackIndex,
            0.25f,           // 0.25초에 발생
            0.0f,           // 즉시 Notify
            FName("Attack"),
            AttackStartIndex
        );
    }

    // Notify 이벤트 가져오기 및 타입 설정
    FAnimNotifyEvent* NotifyEvent = AnimSequence->GetNotifyEvent(0);
    NotifyEvent->SetAnimNotify(FObjectFactory::ConstructObject<UAnimSoundNotify>(nullptr));

    if (NotifyEvent)
    {
        UAnimNotify* BaseNotify = NotifyEvent->GetNotify();
        UAnimSoundNotify* SoundNotify = Cast<UAnimSoundNotify>(BaseNotify);

        if (SoundNotify)
        {
            // 사운드 이름 설정
            SoundNotify->SetSoundName(FName("Attack"));
        }
    }
}

void RabbitAnimInstance::AddAttackNotify(ARabbitEnemy* OutEnemy)
{
    int32 OverlapTrack;
    bool bTrackAdded = Cast<UAnimSequence>(Attack)->AddNotifyTrack(OutEnemy->GetFName(), OverlapTrack);
    UAnimSequence* AnimSequence = Cast<UAnimSequence>(Attack);
    int32 AttackStartIndex;
    int32 AttackEndIndex;
    if (bTrackAdded)
    {
        // 공격 시작 Notify 추가
        AnimSequence->AddNotifyEvent(
            OverlapTrack,
            0.25f,           // 0.25초에 발생
            0.0f,           // 즉시 Notify
            FName("AttackOverlap"),
            AttackStartIndex
        );

        AnimSequence->AddNotifyEvent(
            OverlapTrack,
            0.7f,
            0.0f,
            FName("AttackEnd"),
            AttackEndIndex
        );
    }
    // Notify 이벤트 가져오기 및 타입 설정
    //int AttackStartNotifyIndex = AnimSequence->AnimNotifyTracks[OverlapTrack].NotifyIndices[AttackStartIndex];
    FAnimNotifyEvent* NotifyEvent = AnimSequence->GetNotifyEvent(AttackStartIndex);
    UAnimAttackNotify* AttackNotifyEvent = FObjectFactory::ConstructObject<UAnimAttackNotify>(this);
    NotifyEvent->SetAnimNotify(AttackNotifyEvent);

    USkeletalMeshComponent* SkelMeshComp = GetSkelMeshComponent();
    AActor* Owner = GetSkelMeshComponent()->GetOwner();
    //ARabbitEnemy* RabbitEnemy = Cast<ARabbitEnemy>(Owner);
    ARabbitEnemy* RabbitEnemy = OutEnemy;

    if (NotifyEvent)
    {
        UAnimNotify* BaseNotify = NotifyEvent->GetNotify();
        UAnimAttackNotify* AttackNotify = Cast<UAnimAttackNotify>(BaseNotify);
        if (AttackNotify)
        {
            // 공격 델리게이트 설정
            AttackNotify->OnAttackDelegate.AddUObject(RabbitEnemy, &ARabbitEnemy::OnAttackNotify);
        }
    }

    //int AttackEndNotifyIndex = AnimSequence->AnimNotifyTracks[OverlapTrack].NotifyIndices[AttackEndIndex];
    FAnimNotifyEvent* NotifyAttakEndEvent = AnimSequence->GetNotifyEvent(AttackEndIndex);
    UAnimAttackNotify* AttackNotifyEndEvent = FObjectFactory::ConstructObject<UAnimAttackNotify>(this);
    NotifyAttakEndEvent->SetAnimNotify(AttackNotifyEndEvent);
    if (NotifyAttakEndEvent)
    {
        UAnimNotify* BaseNotify = NotifyAttakEndEvent->GetNotify();
        UAnimAttackNotify* AttackNotifyEnd = Cast<UAnimAttackNotify>(BaseNotify);
        if (AttackNotifyEnd)
        {
            // 공격 종료 델리게이트 설정
            AttackNotifyEnd->OnAttackDelegate.AddUObject(RabbitEnemy, &ARabbitEnemy::OnAttackEndNotify);
        }
    }

}

void RabbitAnimInstance::SetAnimState(EAnimState InAnimState)
{
}

UAnimSequence* RabbitAnimInstance::GetAnimForState(EAnimState InAnimState)
{
    return nullptr;
}
