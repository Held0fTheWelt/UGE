#pragma once

#include "CoreMinimal.h"
#include "ArrowComponentDefinition.generated.h"

/**
 * FSpawnPointsCollection
 *
 * Container for a group of spawn points of a single type (e.g., player, enemy, camera, etc.).
 */
USTRUCT(BlueprintType)
struct FArrowComponentDefinition
{
	GENERATED_BODY()

public:
	FArrowComponentDefinition()
	{
		ComponentName = NAME_None;
		RelativeTransform = FTransform::Identity;
		bUsesSpecialColorAndSize = false; // Default to not using special color and size
		SpecialColor = FLinearColor::White; // Default color
		SpecialSize = 1.0f; // Default size
		SpecialLength = 80.0f; // Default length
		ComponentToAttach = ""; // Default to no component to attach
		SocketToAttach = ""; // Default to no socket to attach
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	FName ComponentName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	FTransform RelativeTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	FName ComponentToAttach;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	FName SocketToAttach;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	bool bVisible = true; // Whether the component should be visible
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	bool bHiddenInGame = false; // Whether the component should be hidden in game

	// Whether to use a special color or not
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	bool bUsesSpecialColorAndSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default, meta = (EditCondition = "bUsesSpecialColorAndSize", EditConditionHides))
	FLinearColor SpecialColor = FLinearColor::White;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default, meta = (EditCondition = "bUsesSpecialColorAndSize", EditConditionHides))
	float SpecialSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default, meta = (EditCondition = "bUsesSpecialColorAndSize", EditConditionHides))
	float SpecialLength;
};