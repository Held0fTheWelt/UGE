#pragma once

#include "CollectionSpawnType.generated.h"

UENUM(BlueprintType)
enum class ECollectionSpawnType : uint8 {
    CST_NONE             UMETA(DisplayName = "None"),
    CST_DUMMY            UMETA(DisplayName = "A dummy actor implementing only cosmetic and raw functionality"),
    CST_DEFAULT          UMETA(DisplayName = "A fully implemented actor"),
    CST_DESTROYED        UMETA(DisplayName = "A destroyed version of this actor"),
    CST_VARIANT          UMETA(DisplayName = "Another not defined variant"),
};