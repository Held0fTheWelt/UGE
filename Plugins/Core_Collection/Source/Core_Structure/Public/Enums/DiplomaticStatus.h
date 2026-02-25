#pragma once

#include "CoreMinimal.h"
#include "DiplomaticStatus.generated.h"

UENUM(BlueprintType)
enum class EDiplomaticStatus : uint8 {
    DS_NONE             UMETA(DisplayName = "None"),
    DS_UNIDENTIFIED     UMETA(DisplayName = "Unidentified"),
    DS_SELF             UMETA(DisplayName = "Self"),
    DS_ALLY             UMETA(DisplayName = "Ally"),
    DS_NEUTRAL          UMETA(DisplayName = "Neutral"),
    DS_ENEMY            UMETA(DisplayName = "Enemy"),
    DS_SELECTED         UMETA(DisplayName = "Selected"),
};
