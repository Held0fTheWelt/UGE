#pragma once

#include "AircraftLight.generated.h"

UENUM(BlueprintType)
enum class EAircraftLight : uint8 {
    AL_NONE         UMETA(DisplayName = "None"),
    AL_BEACON       UMETA(DisplayName = "Beacon"),
    AL_NAVIGATION   UMETA(DisplayName = "Navigation"),
    AL_STROBE       UMETA(DisplayName = "Strobe"),
    AL_SEARCHLIGHT   UMETA(DisplayName = "Searchlight"),

};