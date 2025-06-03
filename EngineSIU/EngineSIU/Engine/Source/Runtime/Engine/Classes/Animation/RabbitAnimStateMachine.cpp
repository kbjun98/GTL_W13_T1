#include "RabbitAnimStateMachine.h"
#include <AnimInstance/RabbitAnimInstance.h>
#include <UObject/Casts.h>
#include "Animation/AnimSequence.h"

void RabbitAnimStateMachine::ProcessState(ERabbitAnimState State)
{
    // GetOuter로 RabbitAnimInstance 접근
    RabbitAnimInstance* AnimInstance = Cast<RabbitAnimInstance>(GetOuter());
    if (!AnimInstance)
    {
        return;
    }

    UAnimSequence* DesiredAnim = nullptr;

    switch (State)
    {
    case ERabbitAnimState::Idle:
        DesiredAnim = Cast<UAnimSequence>(AnimInstance->Idle);
        break;

    case ERabbitAnimState::Walk:
        DesiredAnim = Cast<UAnimSequence>(AnimInstance->Walk);
        break;
    case ERabbitAnimState::Attack:
        DesiredAnim = Cast<UAnimSequence>(AnimInstance->Attack);
        break;
    case ERabbitAnimState::Chase:
        DesiredAnim = Cast<UAnimSequence>(AnimInstance->Chase);
        break;
    }

    if (!DesiredAnim || AnimInstance->GetCurrAnim() == DesiredAnim)
    {
        return;
    }

    float CurrAnimLength = AnimInstance->GetCurrAnim()->GetPlayLength();
    float PrevAnimTimeSnapshot = FMath::Fmod(AnimInstance->GetElapsedTime(), CurrAnimLength);

    // 상태 전환 시 블렌딩용 정보 갱신
    AnimInstance->SetPrevAnim(AnimInstance->GetCurrAnim());
    AnimInstance->SetCurrAnim(DesiredAnim);
    AnimInstance->SetPrevAnimTimeSnapshot(PrevAnimTimeSnapshot); // 이 함수 추가 필요
    AnimInstance->SetElapsedTime(0.f);
    AnimInstance->SetBlendStartTime(0.f);
    AnimInstance->SetIsBlending(true);  
}

FString RabbitAnimStateMachine::GetStateName(ERabbitAnimState State) const
{
    return FString();
}


