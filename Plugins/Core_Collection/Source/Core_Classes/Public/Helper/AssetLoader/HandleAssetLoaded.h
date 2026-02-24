#pragma once

#include "CoreMinimal.h"

#include "HandleAssetLoaded.generated.h"

UCLASS()
class CORE_CLASSES_API UHandleAssetLoaded : public UObject
{
    GENERATED_BODY()

public:
    static int32 GetIndexOfObject(UObject* LoadedObject, const TArray<FName>& InputArray);
};