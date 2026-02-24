#pragma once

#include "CoreMinimal.h"
#include "Enums/ArrowComponentType.h"
#include "ArrowComponentSetupDefinition.generated.h"

/**
 * FSpawnPointsCollection
 *
 * Container for a group of spawn points of a single type (e.g., player, enemy, camera, etc.).
 */
USTRUCT(BlueprintType)
struct FArrowComponentSetupDefinition
{
	GENERATED_BODY()

public:
	FArrowComponentSetupDefinition()
		: Type(EArrowComponentType::ACT_NONE),
		  Color(FLinearColor::White), // Default color
		  Size(1.0f), // Default size
		  Length(80.0f) // Default length
	{
	}

	/** Type/category of these spawn points (see ESpawnPointType) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	EArrowComponentType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	FLinearColor Color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	float Size;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	float Length;
};