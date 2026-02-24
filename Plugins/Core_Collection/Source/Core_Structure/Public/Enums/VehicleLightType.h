#pragma once

#include "CoreMinimal.h"
#include "VehicleLightType.generated.h"

UENUM(BlueprintType)
enum class EVehicleLightType : uint8 {
    VLT_NONE        UMETA(DisplayName = "None"),
    VLT_FRONT       UMETA(DisplayName = "Front Light"),
    VLT_REAR        UMETA(DisplayName = "Rear Light"),
    VLT_BLINK_FRONT UMETA(DisplayName = "Blink Light Front"),
    VLT_BLINK_REAR  UMETA(DisplayName = "Blink Light Rear"),
};
