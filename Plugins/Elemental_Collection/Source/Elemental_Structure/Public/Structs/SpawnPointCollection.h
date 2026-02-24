#pragma once

#include "CoreMinimal.h"
#include "Enums/SpawnPointType.h"
#include "SpawnPointCollection.generated.h"

/**
 * FSpawnPointsCollection
 *
 * Container for a group of spawn points of a single type (e.g., player, enemy, camera, etc.).
 */
USTRUCT(BlueprintType)
struct FSpawnPointsCollection
{
	GENERATED_BODY()

public:
	FSpawnPointsCollection()
		: SpawnPointType(ESpawnPointType::SPT_NONE)
		, SpawnPoints()
	{
	}

	/** Type/category of these spawn points (see ESpawnPointType) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Points")
	ESpawnPointType SpawnPointType;

	/** List of world transforms (locations + rotations) for spawn points of this type */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Points")
	TArray<FTransform> SpawnPoints;
};
