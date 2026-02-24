// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterGroundInfoProviderInterface.generated.h"

/**
 * Implemented by pawns or movement components to provide ground distance (and similar) for animation.
 * Allows GameFeatures (e.g. AnimationSystem) to read ground info without depending on Core_Collection.
 */
UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UCharacterGroundInfoProviderInterface : public UInterface
{
	GENERATED_BODY()
};

class ELEMENTAL_INTERFACES_API ICharacterGroundInfoProviderInterface
{
	GENERATED_BODY()

public:
	/** Returns distance to ground (e.g. from movement component). Returns < 0 if not available. */
	UFUNCTION(BlueprintCallable, Category = "Character|Ground")
	virtual float GetGroundDistance() const { return -1.0f; }
};
