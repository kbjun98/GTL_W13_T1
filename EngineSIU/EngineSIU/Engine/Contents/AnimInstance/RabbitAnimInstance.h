#pragma once

#include "Animation/AnimInstance.h"

class RabbitAnimStateMachine;

class RabbitAnimInstance : public UAnimInstance
{
    DECLARE_CLASS(RabbitAnimInstance, UAnimInstance)

public:
    RabbitAnimInstance();

    virtual void NativeInitializeAnimation() override;

    virtual void NativeUpdateAnimation(float DeltaSeconds, FPoseContext& OutPose) override;

    void SetPlaying(bool bIsPlaying)
    {
        bPlaying = bIsPlaying;
    }

    bool IsPlaying() const
    {
        return bPlaying;
    }

    void SetReverse(bool bIsReverse)
    {
        bReverse = bIsReverse;
    }

    bool IsReverse() const
    {
        return bReverse;
    }

    void SetLooping(bool bIsLooping)
    {
        bLooping = bIsLooping;
    }

    bool IsLooping() const
    {
        return bLooping;
    }

    void SetElapsedTime(float InElapsedTime)
    {
        ElapsedTime = InElapsedTime;
    }

    float GetElapsedTime() const
    {
        return ElapsedTime;
    }

    float GetPlayRate() const
    {
        return PlayRate;
    }

    void SetPlayRate(float InPlayRate)
    {
        PlayRate = InPlayRate;
    }

    int32 GetLoopStartFrame() const
    {
        return LoopStartFrame;
    }

    void SetLoopStartFrame(int32 InLoopStartFrame)
    {
        LoopStartFrame = InLoopStartFrame;
    }

    int32 GetLoopEndFrame() const
    {
        return LoopEndFrame;
    }

    void SetLoopEndFrame(int32 InLoopEndFrame)
    {
        LoopEndFrame = InLoopEndFrame;
    }

    int GetCurrentKey() const
    {
        return CurrentKey;
    }

    void SetCurrentKey(int InCurrentKey)
    {
        CurrentKey = InCurrentKey;
    }

    virtual void SetAnimState(EAnimState InAnimState);

    UAnimSequence* GetAnimForState(EAnimState InAnimState);

  
    virtual UAnimSequence* GetCurrAnim() const { return CurrAnim; }
    void SetCurrAnim(UAnimSequence* AnimSeq) { CurrAnim = AnimSeq; }

    virtual UAnimSequence* GetPrevAnim() const { return PrevAnim; }
    void SetPrevAnim(UAnimSequence* Animseq) { PrevAnim = Animseq; }

    virtual float GetBlendDuration() const { return BlendDuration; }

    virtual void SetBlendDuration(float InBlendDuration) { BlendDuration = InBlendDuration; }

    void SetBlendStartTime(float Time) { BlendStartTime = Time; }

    void SetIsBlending(bool State) { bIsBlending = State; }

    UAnimationAsset* Idle;
    UAnimationAsset* Walk;
    UAnimationAsset* Attack;
private:

    float ElapsedTime;

    float PlayRate;

    bool bLooping;

    bool bPlaying;

    bool bReverse;

    int32 LoopStartFrame;

    int32 LoopEndFrame;

    int CurrentKey;

    UAnimSequence* PrevAnim;

    UAnimSequence* CurrAnim;

    float BlendAlpha;

    float BlendStartTime;

    float BlendDuration;

    bool bIsBlending;

    RabbitAnimStateMachine* StateMachine;
};

