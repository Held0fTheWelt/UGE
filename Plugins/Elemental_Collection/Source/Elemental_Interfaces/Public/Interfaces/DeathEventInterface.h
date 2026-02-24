// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "DeathEventInterface.generated.h"

class AActor;

/**
 * Interface for actors that can receive death events from Health GameFeature
 * This allows the Character to react to death without directly depending on Health Component
 */
UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UDeathEventInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for actors that can receive death events
 * Health GameFeature will call these methods when death events occur
 */
class ELEMENTAL_INTERFACES_API IDeathEventInterface
{
	GENERATED_BODY()

public:
	/**
	 * Called when the death sequence has started.
	 * Character should disable movement and collision here.
	 */
	virtual void OnDeathStarted(AActor* OwningActor) {}

	/**
	 * Called when the death sequence has finished.
	 * Character should handle cleanup and destruction here.
	 */
	virtual void OnDeathFinished(AActor* OwningActor) {}
};
