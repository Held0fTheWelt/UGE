#pragma once

#include "CoreMinimal.h"
#include "CatchableSceneComponent.generated.h"

/**
 * FSpawnPointsCollection
 *
 * Container for a group of spawn points of a single type (e.g., player, enemy, camera, etc.).
 */
USTRUCT(BlueprintType)
struct FCatchableSceneComponent
{
	GENERATED_BODY()

public:
	FCatchableSceneComponent()
	{
	}

	/** Type/category of these spawn points (see ESpawnPointType) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Points")
	FName ComponentName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Points")
	TSoftClassPtr<class UClass> ComponentClass;
};