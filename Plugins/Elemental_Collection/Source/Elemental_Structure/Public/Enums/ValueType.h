#pragma once

#include "CoreMinimal.h"
#include "ValueType.generated.h"

UENUM(BlueprintType)
enum class EValueType : uint8 {
    VT_NONE             UMETA(DisplayName = "None"),
    VT_BOOL             UMETA(DisplayName = "Bool"),
    VT_INT              UMETA(DisplayName = "Integer"),
    VT_FLOAT            UMETA(DisplayName = "Float"),
    VT_STRING           UMETA(DisplayName = "String"),
    VT_ENUM             UMETA(DisplayName = "Enum"),
    VT_STRUCT           UMETA(DisplayName = "Struct"),
    VT_CLASS            UMETA(DisplayName = "Class"),
    VT_COLOR            UMETA(DisplayName = "Color"),
    VT_EMPTY            UMETA(DisplayName = "Empty Line"),
};