// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection / Elemental

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AudioVolumeBusesProviderInterface.generated.h"

/**
 * Interface for providing volume control buses to Core settings.
 * Implemented by the AudioSystem plugin (e.g. UAudioMixEffectsSubsystem).
 * Core and other modules use this interface so they do not depend on the concrete AudioSystem module.
 * Returned objects are USoundControlBus* (Engine/AudioModulation); caller may cast from UObject*.
 */
UINTERFACE(MinimalAPI, BlueprintType)
class UAudioVolumeBusesProviderInterface : public UInterface
{
	GENERATED_BODY()
};

class ELEMENTAL_INTERFACES_API IAudioVolumeBusesProviderInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Audio|VolumeBuses")
	UObject* GetOverallVolumeControlBus() const;
	virtual UObject* GetOverallVolumeControlBus_Implementation() const { return nullptr; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Audio|VolumeBuses")
	UObject* GetMusicVolumeControlBus() const;
	virtual UObject* GetMusicVolumeControlBus_Implementation() const { return nullptr; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Audio|VolumeBuses")
	UObject* GetSoundFXVolumeControlBus() const;
	virtual UObject* GetSoundFXVolumeControlBus_Implementation() const { return nullptr; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Audio|VolumeBuses")
	UObject* GetDialogueVolumeControlBus() const;
	virtual UObject* GetDialogueVolumeControlBus_Implementation() const { return nullptr; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Audio|VolumeBuses")
	UObject* GetVoiceChatVolumeControlBus() const;
	virtual UObject* GetVoiceChatVolumeControlBus_Implementation() const { return nullptr; }
};
