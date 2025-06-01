#include "InputComponent.h"

void UInputComponent::ProcessInput(float DeltaTime)
{
    ProcessKeyInput(DeltaTime);
    ProcessAxisInput(DeltaTime);
}

void UInputComponent::SetPossess()
{
    BindInputDelegate();

    //TODO: Possess일때 기존에 있던거 다시 넣어줘야할수도
}

void UInputComponent::BindInputDelegate()
{
    FSlateAppMessageHandler* Handler = GEngineLoop.GetAppMessageHandler();

    BindKeyDownDelegateHandles.Add(Handler->OnKeyDownDelegate.AddLambda([this](const FKeyEvent& InKeyEvent)
        {
            InputKey(InKeyEvent);
        }));

    BindKeyUpDelegateHandles.Add(Handler->OnKeyUpDelegate.AddLambda([this](const FKeyEvent& InKeyEvent)
        {
            InputKey(InKeyEvent);
        }));

    BindMouseMoveDelegateHandles.Add(Handler->OnMouseMoveDelegate.AddLambda([this](const FPointerEvent& InMouseEvent)
        {
        }));

    BindMouseDownDelegateHandles.Add(Handler->OnMouseDownDelegate.AddLambda([this](const FPointerEvent& InMouseEvent)
        {
            InputMouse(InMouseEvent);
        }));

    BindMouseUpDelegateHandles.Add(Handler->OnMouseUpDelegate.AddLambda([this](const FPointerEvent& InMouseEvent)
        {
            InputMouse(InMouseEvent);
        }));

    BindRawMouseMoveDelegateHandles.Add(Handler->OnRawMouseInputDelegate.AddLambda([this](const FPointerEvent& InMouseEvent)
        {
            MouseDelta = InMouseEvent.GetCursorDelta();
        }));

}

void UInputComponent::UnPossess()
{
    ClearBindDelegate();
}

void UInputComponent::ClearBindDelegate()
{
    FSlateAppMessageHandler* Handler = GEngineLoop.GetAppMessageHandler();

    for (FDelegateHandle DelegateHandle : BindKeyDownDelegateHandles)
    {
        Handler->OnKeyDownDelegate.Remove(DelegateHandle);
    }

    for (FDelegateHandle DelegateHandle : BindKeyUpDelegateHandles)
    {
        Handler->OnKeyUpDelegate.Remove(DelegateHandle);
    }
    for (FDelegateHandle DelegateHandle : BindMouseMoveDelegateHandles)
    {
        Handler->OnMouseMoveDelegate.Remove(DelegateHandle);
    }
    for (FDelegateHandle DelegateHandle : BindMouseDownDelegateHandles)
    {
        Handler->OnMouseDownDelegate.Remove(DelegateHandle);
    }
    for (FDelegateHandle DelegateHandle : BindMouseUpDelegateHandles)
    {
        Handler->OnMouseUpDelegate.Remove(DelegateHandle);
    }
    for (FDelegateHandle DelegateHandle : BindRawMouseMoveDelegateHandles)
    {
        Handler->OnRawMouseInputDelegate.Remove(DelegateHandle);
    }

    BindKeyDownDelegateHandles.Empty();
    BindKeyUpDelegateHandles.Empty();
    BindMouseMoveDelegateHandles.Empty();
    BindMouseDownDelegateHandles.Empty();
    BindMouseUpDelegateHandles.Empty();
    BindRawMouseMoveDelegateHandles.Empty();
}

void UInputComponent::InputKey(const FKeyEvent& InKeyEvent)
{
    // 일반적인 단일 키 이벤트
    switch (InKeyEvent.GetCharacter())
    {
    case 'W':
    {
        if (InKeyEvent.GetInputEvent() == IE_Pressed)
        {
            PressedKeys.Add(EKeys::W);
        }
        else if (InKeyEvent.GetInputEvent() == IE_Released)
        {
            PressedKeys.Remove(EKeys::W);
        }
        break;
    }
    case 'A':
    {
        if (InKeyEvent.GetInputEvent() == IE_Pressed)
        {
            PressedKeys.Add(EKeys::A);
        }
        else if (InKeyEvent.GetInputEvent() == IE_Released)
        {
            PressedKeys.Remove(EKeys::A);
        }
        break;
    }
    case 'S':
    {
        if (InKeyEvent.GetInputEvent() == IE_Pressed)
        {
            PressedKeys.Add(EKeys::S);
        }
        else if (InKeyEvent.GetInputEvent() == IE_Released)
        {
            PressedKeys.Remove(EKeys::S);
        }
        break;
    }
    case 'D':
    {
        if (InKeyEvent.GetInputEvent() == IE_Pressed)
        {
            PressedKeys.Add(EKeys::D);
        }
        else if (InKeyEvent.GetInputEvent() == IE_Released)
        {
            PressedKeys.Remove(EKeys::D);
        }
        break;
    }
    default:
        break;
    }

    switch (InKeyEvent.GetKeyCode())
    {
        case VK_ESCAPE: // Escape 키
        {
            if (InKeyEvent.GetInputEvent() == IE_Pressed)
            {
                PressedKeys.Add(EKeys::Escape);
                KeyBindDelegate[FString("ESC_Pressed")].Broadcast(0);
            }
            else if (InKeyEvent.GetInputEvent() == IE_Released)
            {
                PressedKeys.Remove(EKeys::Escape);
            }
            break;
        }
        default:
            break;
    }

}

void UInputComponent::InputMouse(const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
    {
        if (InMouseEvent.GetInputEvent() == IE_Pressed)
        {
            PressedKeys.Add(EKeys::RightMouseButton);
            KeyBindDelegate[FString("R_Pressed")].Broadcast(0);
        }
        else if (InMouseEvent.GetInputEvent() == IE_Released)
        {
            PressedKeys.Remove(EKeys::RightMouseButton);
            KeyBindDelegate[FString("R_Released")].Broadcast(0);
        }
    }
    else if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        if (InMouseEvent.GetInputEvent() == IE_Pressed)
        {
            PressedKeys.Add(EKeys::LeftMouseButton);
            KeyBindDelegate[FString("L_Pressed")].Broadcast(0);
        }
        else if (InMouseEvent.GetInputEvent() == IE_Released)
        {
            PressedKeys.Remove(EKeys::LeftMouseButton);
            KeyBindDelegate[FString("L_Released")].Broadcast(0);
        }
    }
    else if (InMouseEvent.GetEffectingButton() == EKeys::MiddleMouseButton)
    {
        if (InMouseEvent.GetInputEvent() == IE_Pressed)
        {
            PressedKeys.Add(EKeys::MiddleMouseButton);
        }
        else if (InMouseEvent.GetInputEvent() == IE_Released)
        {
            PressedKeys.Remove(EKeys::MiddleMouseButton);
        }
    }

}

void UInputComponent::ProcessKeyInput(float DeltaTime)
{
    if (PressedKeys.Contains(EKeys::W))
    {
        KeyBindDelegate[FString("W")].Broadcast(DeltaTime);
    }
    if (PressedKeys.Contains(EKeys::A))
    {
        KeyBindDelegate[FString("A")].Broadcast(DeltaTime);
    }
    if (PressedKeys.Contains(EKeys::S))
    {
        KeyBindDelegate[FString("S")].Broadcast(DeltaTime);
    }
    if (PressedKeys.Contains(EKeys::D))
    {
        KeyBindDelegate[FString("D")].Broadcast(DeltaTime);
    }
    if (PressedKeys.Contains(EKeys::Escape))
    {
        KeyBindDelegate[FString("ESC")].Broadcast(DeltaTime);
    }
    if (PressedKeys.Contains(EKeys::LeftMouseButton))
    {
        KeyBindDelegate[FString("L_Pressing")].Broadcast(DeltaTime);
    }
    if (PressedKeys.Contains(EKeys::RightMouseButton))
    {
        KeyBindDelegate[FString("R_Pressing")].Broadcast(DeltaTime);
    }
}

void UInputComponent::ProcessAxisInput(float DeltaTime)
{
    if (MouseDelta.X != 0.0f)
    {
        AxisBindDelegate[FString("Turn")].Broadcast(MouseDelta.X * DeltaTime);
    }
    if (MouseDelta.Y != 0.0f)
    {
        AxisBindDelegate[FString("LookUp")].Broadcast(MouseDelta.Y * DeltaTime);
    }

    MouseDelta = FVector2D::ZeroVector; // Reset after processing
}


void UInputComponent::BindAction(const FString& Key, const std::function<void(float)>& Callback)
{
    if (Callback == nullptr)
    {
        return;
    }

    KeyBindDelegate[Key].AddLambda([this, Callback](float DeltaTime)
        {
            Callback(DeltaTime);
        });
}

void UInputComponent::BindAxis(const FString& Key, const std::function<void(float)>& Callback)
{
    if (Callback == nullptr)
    {
        return;
    }

    AxisBindDelegate[Key].AddLambda([this, Callback](float DeltaTime)
        {
            Callback(DeltaTime);
        });
}
