// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"

/**
 * Registry for the interaction trace/collision channel so GameFeatures (e.g. InteractionSystem)
 * can query it without depending on Core_Physics. Core_Physics sets the channel at startup.
 */
namespace InteractionCollisionChannelRegistry
{
	/** Set the channel (e.g. from Core_Physics module startup). */
	ELEMENTAL_INTERFACES_API void Set(ECollisionChannel Channel);

	/** Get the channel. Returns ECC_WorldDynamic if none registered. */
	ELEMENTAL_INTERFACES_API ECollisionChannel Get();
}
