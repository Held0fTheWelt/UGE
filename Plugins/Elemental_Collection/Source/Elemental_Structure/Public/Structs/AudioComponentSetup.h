#pragma once

#include "CoreMinimal.h"
#include "Enums/SpawnPointType.h"
#include "Enums/SoundSourceType.h"
#include "AudioComponentSetup.generated.h"

/**
 * FSpawnPointsCollection
 *
 * Container for a group of spawn points of a single type (e.g., player, enemy, camera, etc.).
 */
USTRUCT(BlueprintType)
struct FAudioComponentSetup
{
	GENERATED_BODY()

public:
	FAudioComponentSetup()
	{
	}

	// Name of the audio component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	FName ComponentName;

	// Transform for the audio component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	FTransform ComponentTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	ESoundSourceType SoundSourceType = ESoundSourceType::SST_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	float FadeInOutTime = 3.0f; // Time to fade in the audio component

	// Default volume multiplier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	float VolumeMultiplier = 1.0f; 
	// Default pitch multiplier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	float PitchMultiplier = 1.0f; 
	// Whether the audio component should start playing automatically
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	bool bAutoActivate = false; 
	// Whether the audio component should loop playback
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	bool bLooping = true; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	bool bIsUISound = false; 
	
	// The sound asset to play
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	TSoftObjectPtr<class USoundBase> Sound; 

	// Optional attenuation settings for the audio component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	TSoftObjectPtr<class USoundAttenuation> AttenuationSettings;

};