// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection / Elemental

#include "Subsystems/ElementalAudioProviderSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ElementalAudioProviderSubsystem)

void UElementalAudioProviderSubsystem::RegisterAudioSystem(UObject* InProvider)
{
	AudioSystemProvider = InProvider;
}

void UElementalAudioProviderSubsystem::UnregisterAudioSystem()
{
	AudioSystemProvider = nullptr;
}

TScriptInterface<IAudioSystemInterface> UElementalAudioProviderSubsystem::GetAudioSystem() const
{
	return TScriptInterface<IAudioSystemInterface>(AudioSystemProvider);
}
