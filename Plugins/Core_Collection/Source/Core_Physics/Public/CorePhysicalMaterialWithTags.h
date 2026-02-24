// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "GameplayTagContainer.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Interfaces/PhysicalMaterialWithTagsInterface.h"

#include "CorePhysicalMaterialWithTags.generated.h"

class UObject;

/**
 * UCorePhysicalMaterialWithTags
 *
 * A physical material that includes gameplay tags for game code to reason about the material.
 * Implements IPhysicalMaterialWithTagsInterface so Core_AbilitySystem can use tags without depending on Core_Physics.
 */
UCLASS(meta = (Interfaces = "PhysicalMaterialWithTagsInterface"))
class CORE_PHYSICS_API UCorePhysicalMaterialWithTags : public UPhysicalMaterial, public IPhysicalMaterialWithTagsInterface
{
	GENERATED_BODY()

public:
	UCorePhysicalMaterialWithTags(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// A container of gameplay tags that game code can use to reason about this physical material
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=PhysicalProperties)
	FGameplayTagContainer Tags;

	//~IPhysicalMaterialWithTagsInterface
	virtual FGameplayTagContainer GetTags() const override { return Tags; }
};
