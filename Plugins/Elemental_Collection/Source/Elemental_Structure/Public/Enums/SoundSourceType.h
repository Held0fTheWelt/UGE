#pragma once

#include "CoreMinimal.h"
#include "SoundSourceType.generated.h"

UENUM(BlueprintType)
enum class ESoundSourceType : uint8 {
    SST_NONE			UMETA(DisplayName = "None"),
	SST_ENGINE_OUTSIDE	UMETA(DisplayName = "Engine Sound Source Outside"),
	SST_ENGINE_INSIDE	UMETA(DisplayName = "Engine Sound Source Inside"),
	SST_ROTOR			UMETA(DisplayName = "Rotor Sound Source"),
	SST_SYSTEM			UMETA(DisplayName = "System Sound Source"),
};