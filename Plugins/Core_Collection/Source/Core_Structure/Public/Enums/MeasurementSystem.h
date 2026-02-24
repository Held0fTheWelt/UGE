#pragma once

#include "CoreMinimal.h"
#include "MeasurementSystem.generated.h"

UENUM(BlueprintType)
enum class EMeasurementSystem : uint8 {
    MS_NONE         UMETA(DisplayName = "None"),
    MS_METRIC       UMETA(DisplayName = "Metric System"),
    MS_IMPERIAL     UMETA(DisplayName = "Imperial System"),
};
