// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection / Elemental

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AudioMixEffectsInterface.generated.h"

/**
 * Interface for the audio mix effects subsystem (provided by the AudioSystem plugin).
 * Core and other modules use this interface so they do not depend on the concrete AudioSystem module.
 * Use UElementalAudioProviderSubsystem::GetAudioMixEffects() to obtain the implementation.
 */
UINTERFACE(MinimalAPI, BlueprintType)
class UAudioMixEffectsInterface : public UInterface
{
	GENERATED_BODY()
};

class ELEMENTAL_INTERFACES_API IAudioMixEffectsInterface
{
	GENERATED_BODY()

public:
	/** Apply HDR or LDR submix effect chains based on user preference. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Audio|MixEffects")
	void ApplyDynamicRangeEffectsChains(bool bHDRAudio);
	virtual void ApplyDynamicRangeEffectsChains_Implementation(bool bHDRAudio) {}

	/** Enable or disable the loading screen mix (e.g. to duck background audio during loading). */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Audio|MixEffects")
	void SetLoadingScreenMixActive(bool bActive);
	virtual void SetLoadingScreenMixActive_Implementation(bool bActive) {}
};
