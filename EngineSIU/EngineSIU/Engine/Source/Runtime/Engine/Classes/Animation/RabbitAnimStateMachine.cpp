#include "RabbitAnimStateMachine.h"
#include <AnimInstance/RabbitAnimInstance.h>
#include <UObject/Casts.h>
#include "Animation/AnimSequence.h"

RabbitAnimStateMachine::RabbitAnimStateMachine()
{
    CurrentState = ERabbitState::EIDLE;
}

void RabbitAnimStateMachine::ProcessState()
{
    // GetOuter로 RabbitAnimInstance 접근
    RabbitAnimInstance* AnimInstance = Cast<RabbitAnimInstance>(GetOuter());
    if (!AnimInstance)
        return;

    UAnimSequence* DesiredAnim = nullptr;

    switch (CurrentState)
    {
    case ERabbitState::EIDLE:
        DesiredAnim = Cast<UAnimSequence>(AnimInstance->Idle);
        break;

    case ERabbitState::EWALK:
        DesiredAnim = Cast<UAnimSequence>(AnimInstance->Walk);
        break;

    case ERabbitState::EAttack:
        DesiredAnim = Cast<UAnimSequence>(AnimInstance->Attack);
        break;
    }

    if (!DesiredAnim || AnimInstance->GetCurrAnim() == DesiredAnim)
        return;

    // 상태 전환 시 블렌딩용 정보 갱신
    AnimInstance->SetPrevAnim(AnimInstance->GetCurrAnim());
    AnimInstance->SetCurrAnim(DesiredAnim);
    AnimInstance->SetElapsedTime(0.f);
    AnimInstance->SetBlendStartTime(0.f);
    AnimInstance->SetIsBlending(true);
}

FString RabbitAnimStateMachine::GetStateName(ERabbitState State) const
{
    return FString();
}

ERabbitState RabbitAnimStateMachine::GetState() const
{
    return ERabbitState();
}

void RabbitAnimStateMachine::SetRabbitAnimState(ERabbitState State)
{
    CurrentState = State;
}
