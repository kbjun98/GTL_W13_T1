#include "IdleCameraShake.h"

#include "Camera/PerlinNoiseCameraShakePattern.h"

UIdleCameraShake::UIdleCameraShake()
{
    UPerlinNoiseCameraShakePattern* Pattern = new UPerlinNoiseCameraShakePattern();
    Pattern->Duration = 0.0f;
    Pattern->BlendInTime = 0.f;
    Pattern->BlendOutTime = 0.f;

    // Rotation
    Pattern->RotationAmplitudeMultiplier = 1.f;
    Pattern->RotationFrequencyMultiplier = 1.f;
    
    Pattern->Yaw.Amplitude = 0.3f;
    Pattern->Yaw.Frequency = 0.8f;

    Pattern->Pitch.Amplitude = 0.2f;
    Pattern->Pitch.Frequency = 1.5f;

    Pattern->Roll.Amplitude = 0.f;
    Pattern->Roll.Frequency = 1.f;

    // Location
    Pattern->LocationAmplitudeMultiplier = 1.f;
    Pattern->LocationFrequencyMultiplier = 1.f;

    Pattern->X.Amplitude = 0.f;
    Pattern->X.Frequency = 1.f;

    Pattern->Y.Amplitude = 0.2f;
    Pattern->Y.Frequency = 1.f;

    Pattern->Z.Amplitude = 0.1f;
    Pattern->Z.Frequency = 0.4f;
    
    SetRootShakePattern(Pattern);
}
