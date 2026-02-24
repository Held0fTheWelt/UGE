// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection / Elemental

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AudioSystemInterface.generated.h"

/**
 * Single interface for the audio system (e.g. provided by the AudioSystem plugin).
 * Core_Collection and other modules use only this interface so they do not depend on the AudioSystem plugin.
 * Use UElementalAudioProviderSubsystem::GetAudioSystem() to obtain the implementation.
 * Combines mix effects (HDR/LDR, loading screen) and volume bus access.
 */
UINTERFACE(MinimalAPI, BlueprintType)
class UAudioSystemInterface : public UInterface
{
	GENERATED_BODY()
};

class ELEMENTAL_INTERFACES_API IAudioSystemInterface
{
	GENERATED_BODY()

public:
	// Mix effects (HDR/LDR, loading screen)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Audio|System")
	void ApplyDynamicRangeEffectsChains(bool bHDRAudio);
	virtual void ApplyDynamicRangeEffectsChains_Implementation(bool bHDRAudio) {}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Audio|System")
	void SetLoadingScreenMixActive(bool bActive);
	virtual void SetLoadingScreenMixActive_Implementation(bool bActive) {}

	// Volume buses (returned objects are USoundControlBus*; caller may cast from UObject*)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Audio|System")
	UObject* GetOverallVolumeControlBus() const;
	virtual UObject* GetOverallVolumeControlBus_Implementation() const { return nullptr; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Audio|System")
	UObject* GetMusicVolumeControlBus() const;
	virtual UObject* GetMusicVolumeControlBus_Implementation() const { return nullptr; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Audio|System")
	UObject* GetSoundFXVolumeControlBus() const;
	virtual UObject* GetSoundFXVolumeControlBus_Implementation() const { return nullptr; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Audio|System")
	UObject* GetDialogueVolumeControlBus() const;
	virtual UObject* GetDialogueVolumeControlBus_Implementation() const { return nullptr; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Audio|System")
	UObject* GetVoiceChatVolumeControlBus() const;
	virtual UObject* GetVoiceChatVolumeControlBus_Implementation() const { return nullptr; }
};
