// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CoreSettingsLocalPlayerInterface.generated.h"

/**
 * Interface for a local player that provides Core settings (shared and local).
 * Uses UObject* so Core_Interfaces does not depend on Core_Setup/Core_Data.
 * Implemented by UCoreLocalPlayer (Core_Classes); callers cast to UCoreSettingsLocal/UCoreSettingsShared (Core_Data).
 */
UINTERFACE(MinimalAPI, BlueprintType, Meta = (CannotImplementInterfaceInBlueprint))
class UCoreSettingsLocalPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

class CORE_INTERFACES_API ICoreSettingsLocalPlayerInterface
{
	GENERATED_BODY()

public:
	/** Returns the local (machine-specific) settings for this local player (cast to UCoreSettingsLocal from Core_Data). */
	virtual UObject* GetLocalSettings() const { return nullptr; }

	/** Returns the shared settings for this local player (cast to UCoreSettingsShared from Core_Data). */
	virtual UObject* GetSharedSettingsObject() const { return nullptr; }
};
