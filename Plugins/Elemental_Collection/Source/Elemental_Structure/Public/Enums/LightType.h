#pragma once

#include "CoreMinimal.h"
#include "LightType.generated.h"

UENUM(BlueprintType)
enum class ELightType : uint8 {
    LT_NONE                 UMETA(DisplayName = "None"),
    LT_LIGHT_DIRECTIONAL    UMETA(DisplayName = "Directional Light"),
    LT_LIGHT_SPOT           UMETA(DisplayName = "SpotLight"),
    LT_LIGHT_POINT          UMETA(DisplayName = "PointLight"),
};