// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"

#include "MovementModeTagInterface.generated.h"

/**
 * Interface for actors that can manage movement mode gameplay tags
 * Movement GameFeature will call these methods to update movement tags
 */
UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UMovementModeTagInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for managing movement mode gameplay tags
 * Allows Movement GameFeature to update tags without direct dependency
 */
class ELEMENTAL_INTERFACES_API IMovementModeTagInterface
{
	GENERATED_BODY()

public:
	/**
	 * Sets a movement mode tag based on the current movement mode
	 * @param MovementMode The current movement mode
	 * @param CustomMovementMode The custom movement mode (if any)
	 * @param bTagEnabled Whether the tag should be enabled
	 */
	virtual void SetMovementModeTag(EMovementMode MovementMode, uint8 CustomMovementMode, bool bTagEnabled) {}

	/**
	 * Initializes gameplay tags for movement modes
	 * Called when ability system is initialized
	 */
	virtual void InitializeMovementModeTags() {}

	/**
	 * Sets the crouching status tag count (0 or 1).
	 * Called by HumanoidMovement when crouch state changes; implementation uses game-specific ASC/tags.
	 */
	virtual void SetCrouchingTagCount(int32 Count) {}
};
