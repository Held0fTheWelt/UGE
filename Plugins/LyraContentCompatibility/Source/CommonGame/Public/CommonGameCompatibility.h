#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CommonGameCompatibility.generated.h"

/** Script-package anchor for reviewed Lyra content that imports /Script/CommonGame only. */
UCLASS(NotBlueprintable, Transient)
class COMMONGAME_API UCommonGameCompatibilityAnchor final : public UObject
{
    GENERATED_BODY()
};
