#pragma once

#include "DemoDisplayType.generated.h"

UENUM(BlueprintType)
enum class EDemoDisplayType : uint8 {
    DDT_NONE                UMETA(DisplayName = "None"),
    DDT_ROUND               UMETA(DisplayName = "Round"),
    DDT_SQUAREL             UMETA(DisplayName = "Square L"),
    DDT_ROOML               UMETA(DisplayName = "Room L"),
    DDT_DESCRIPTION_ONLY    UMETA(DisplayName = "Description Only"),
    DDT_SQUAREL_FLAT_WALL   UMETA(DisplayName = "Square L Flat Wall"),
};