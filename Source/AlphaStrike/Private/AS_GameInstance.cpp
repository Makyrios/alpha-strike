// AlphaStrike by Team #1. AlphaNova courses🤙

#include "AS_GameInstance.h"
#include "Sound/SoundClass.h"

void UAS_GameInstance::SetMasterSoundVolume(float Volume)
{
    if (!MasterSoundClass) return;

    Volume = FMath::Clamp(Volume, 0.f, 1.f);
    MasterSoundClass->Properties.Volume = Volume;
}

float UAS_GameInstance::GetMasterSoundVolume() const
{
    if (!MasterSoundClass) return 0.0f;

    return MasterSoundClass->Properties.Volume;
}
