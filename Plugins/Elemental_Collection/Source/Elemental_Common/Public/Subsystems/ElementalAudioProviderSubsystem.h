// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection / Elemental

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Interfaces/AudioSystemInterface.h"
#include "ElementalAudioProviderSubsystem.generated.h"

/**
 * World subsystem that holds a reference to the audio system implementation (e.g. AudioSystem plugin).
 * Core_Collection and other modules use only IAudioSystemInterface via GetAudioSystem() and do not depend on the AudioSystem plugin.
 */
UCLASS()
class ELEMENTAL_COMMON_API UElementalAudioProviderSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	/** Register the object that implements IAudioSystemInterface (e.g. UAudioMixEffectsSubsystem from the AudioSystem plugin). */
	UFUNCTION(BlueprintCallable, Category = "Elemental|Audio")
	void RegisterAudioSystem(UObject* InProvider);

	/** Unregister the audio system provider. */
	UFUNCTION(BlueprintCallable, Category = "Elemental|Audio")
	void UnregisterAudioSystem();

	/** Get the registered audio system; returns nullptr if none registered. Core uses only this and IAudioSystemInterface. */
	UFUNCTION(BlueprintPure, Category = "Elemental|Audio")
	TScriptInterface<IAudioSystemInterface> GetAudioSystem() const;

private:
	UPROPERTY(Transient)
	TObjectPtr<UObject> AudioSystemProvider = nullptr;
};
