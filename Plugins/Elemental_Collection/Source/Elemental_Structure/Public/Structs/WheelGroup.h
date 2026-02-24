#pragma once

#include "CoreMinimal.h"
#include "WheelGroup.generated.h"

USTRUCT(BlueprintType)
struct ELEMENTAL_STRUCTURE_API FWheelGroup
{
	GENERATED_BODY()

public:
	FWheelGroup()
	{
		WheelIndices.Empty();
	}
	FWheelGroup(const TArray<int32>& InWheelIndices)
		: WheelIndices(InWheelIndices)
	{
	}
	UPROPERTY(EditAnywhere, Category = WheelGroup)
	TArray<int32> WheelIndices;
};