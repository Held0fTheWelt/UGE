// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#pragma once

#include "UObject/Interface.h"
#include "GameplayTagContainer.h"

#include "CoreAbilitySourceInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UCoreAbilitySourceInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * ICoreAbilitySourceInterface
 *
 *	Interface for objects that can be used as ability sources.
 *	Adapted from ILyraAbilitySourceInterface.
 */
class CORE_ABILITYSYSTEM_API ICoreAbilitySourceInterface
{
	GENERATED_BODY()

public:
	/** Returns the physical material from the source object, if any */
	virtual const UPhysicalMaterial* GetPhysicalMaterial() const { return nullptr; }

	/** Returns attenuation multiplier based on physical material (default 1.0f) */
	virtual float GetPhysicalMaterialAttenuation(const UPhysicalMaterial* PhysMat, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags) const { return 1.0f; }

	/** Returns attenuation multiplier based on distance (default 1.0f) */
	virtual float GetDistanceAttenuation(float Distance, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags) const { return 1.0f; }
};
