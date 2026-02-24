#pragma once

#include "CoreMinimal.h"
#include "Enums/SpawnPointType.h"
#include "PostProcessConfig.generated.h"

/**
 * FSpawnPointsCollection
 *
 * Container for a group of spawn points of a single type (e.g., player, enemy, camera, etc.).
 */
USTRUCT(BlueprintType)
struct FPostProcessConfig
{
	GENERATED_BODY()

public:
	FPostProcessConfig()
	{
		PostProcessMaterials.Empty();
		bUnbound = false; 
	}

	/** Type/category of these spawn points (see ESpawnPointType) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Points")
	TArray<TSoftObjectPtr<UMaterialInterface>> PostProcessMaterials;

	/** List of world transforms (locations + rotations) for spawn points of this type */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Points")
	bool bUnbound;
};