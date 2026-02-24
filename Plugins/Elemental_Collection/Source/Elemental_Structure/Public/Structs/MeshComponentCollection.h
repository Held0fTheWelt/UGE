#pragma once

#include "CoreMinimal.h"
#include "Enums/MeshComponentType.h"
#include "MeshComponentCollection.generated.h"

/**
 */
USTRUCT()
struct FMeshComponentCollection
{
	GENERATED_BODY()

public:
	FMeshComponentCollection()
	{
	}

	UPROPERTY()
	TMap<FName, TObjectPtr<class USceneComponent>> MeshComponents;
};