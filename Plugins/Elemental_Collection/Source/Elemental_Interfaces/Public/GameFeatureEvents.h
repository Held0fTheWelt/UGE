// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * Game framework extension event names used by GameFeatures and PlayerState.
 * Placed in Elemental_Interfaces so Core_GameFeatures and Elemental_Player
 * can share the constant without Core depending on Elemental_Player.
 */
namespace GameFeatureEvents
{
	/** Fired when the ability system is ready (e.g. after PawnExtension/ASC setup). Used by GameFeatureAction_AddAbilities. */
	inline const FName NAME_AbilityReady(TEXT("AbilitiesReady"));
}
