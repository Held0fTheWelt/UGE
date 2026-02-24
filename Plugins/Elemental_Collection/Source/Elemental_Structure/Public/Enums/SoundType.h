#pragma once

#include "CoreMinimal.h"
#include "SoundType.generated.h"

/** Sound classification for game audio. Named EElementalSoundType to avoid clash with Engine's ESoundType (AudioDeviceManager.h). */
UENUM(BlueprintType)
enum class EElementalSoundType : uint8 {
	ST_NONE     UMETA(DisplayName = "None"),
	ST_INTERIOR UMETA(DisplayName = "Interior Sound"),
	ST_EXTERIOR UMETA(DisplayName = "Exterior Sound"),
	ST_SIGNAL   UMETA(DisplayName = "Signal Sound"),
	ST_UI       UMETA(DisplayName = "User Interface Sound"),
};