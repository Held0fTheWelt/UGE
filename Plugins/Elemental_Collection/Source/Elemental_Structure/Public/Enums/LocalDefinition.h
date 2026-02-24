#pragma once

#include "CoreMinimal.h"
#include "LocalDefinition.generated.h"

UENUM(BlueprintType)
enum class ELocalDefinition : uint8 {
    LD_NONE                 UMETA(DisplayName = "None"),
    LD_INSIDE               UMETA(DisplayName = "Inside"),
    LD_OUTSIDE              UMETA(DisplayName = "Outside"),
};