#pragma once

#include "CoreMinimal.h"
#include "Enums/MeshComponentType.h"
#include "MeshComponentDefinition.generated.h"

/**
 */
USTRUCT(BlueprintType)
struct FMeshComponentDefinition
{
	GENERATED_BODY()

public:
	FMeshComponentDefinition()
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	EMeshComponentType Type = EMeshComponentType::MCT_NONE;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	FName MeshComponentName = NAME_None;
};