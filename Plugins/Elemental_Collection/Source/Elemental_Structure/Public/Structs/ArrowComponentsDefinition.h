#pragma once

#include "CoreMinimal.h"
#include "Enums/ArrowComponentType.h"
#include "Structs/ArrowComponentDefinition.h"
#include "ArrowComponentsDefinition.generated.h"

/**
 * FSpawnPointsCollection
 *
 * Container for a group of spawn points of a single type (e.g., player, enemy, camera, etc.).
 */
USTRUCT(BlueprintType)
struct FArrowComponentsDefinition
{
	GENERATED_BODY()

public:
	FArrowComponentsDefinition()
		: Type(EArrowComponentType::ACT_NONE),
		Components()
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	EArrowComponentType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	TArray<FArrowComponentDefinition> Components;
};