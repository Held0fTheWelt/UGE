#pragma once

#include "HelicopterPhysicalComponentType.generated.h"

UENUM(BlueprintType)
enum class EHelicopterPhysicalComponentType : uint8 {
    HPT_NONE         UMETA(DisplayName = "None"),
    HPT_BODY         UMETA(DisplayName = "Body"),
    HPT_ROTOR_SHAFT  UMETA(DisplayName = "Main Rotor Shaft"),
    HPT_ROTOR_TAIL         UMETA(DisplayName = "Tail"),
};