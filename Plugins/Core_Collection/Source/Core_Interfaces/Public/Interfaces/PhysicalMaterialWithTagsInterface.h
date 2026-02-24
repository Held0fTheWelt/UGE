// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "PhysicalMaterialWithTagsInterface.generated.h"

/**
 * Interface for physical materials that expose gameplay tags (e.g. for applying to gameplay effect specs).
 * Core_Physics's UCorePhysicalMaterialWithTags implements this so Core_AbilitySystem does not depend on Core_Physics.
 */
UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UPhysicalMaterialWithTagsInterface : public UInterface
{
	GENERATED_BODY()
};

class CORE_INTERFACES_API IPhysicalMaterialWithTagsInterface
{
	GENERATED_BODY()

public:
	virtual FGameplayTagContainer GetTags() const { return FGameplayTagContainer(); }
};
