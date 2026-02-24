#pragma once

#include "CoreMinimal.h"
#include "RuntimeMode.generated.h"

UENUM(BlueprintType)
enum class ERuntimeMode : uint8 {
	RM_OFF 		UMETA(DisplayName = "Off"),
	RM_ON 		UMETA(DisplayName = "On"),
	RM_NEXT 	UMETA(DisplayName = "Next"),
	RM_PREVIOUS UMETA(DisplayName = "Previous"),
	RM_RANDOM 	UMETA(DisplayName = "Random"),
	RM_EXACT 	UMETA(DisplayName = "Exact"),
};