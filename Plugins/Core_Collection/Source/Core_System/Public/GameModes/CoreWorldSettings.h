// Copyright Epic Games, Inc. All Rights Reserved.
// Core_Collection

#pragma once

#include "GameFramework/WorldSettings.h"

#include "CoreWorldSettings.generated.h"

/**
 * The default world settings object, used primarily to set the default gameplay experience to use when playing on this map.
 * DefaultGameplayExperience is resolved via AssetManager; use ELEMENTAL_INTERFACES (IExperienceManagerInterface) for runtime access.
 */
UCLASS()
class CORE_SYSTEM_API ACoreWorldSettings : public AWorldSettings
{
	GENERATED_BODY()

public:

	ACoreWorldSettings(const FObjectInitializer& ObjectInitializer);

#if WITH_EDITOR
	virtual void CheckForErrors() override;
#endif

public:
	// Returns the default experience to use when a server opens this map if it is not overridden by the user-facing experience
	FPrimaryAssetId GetDefaultGameplayExperience() const;

protected:
	// The default experience to use when a server opens this map if it is not overridden by the user-facing experience (soft class path; resolved via AssetManager)
	UPROPERTY(EditDefaultsOnly, Category=GameMode)
	TSoftClassPtr<UObject> DefaultGameplayExperience;

public:

#if WITH_EDITORONLY_DATA
	// Is this level part of a front-end or other standalone experience?
	// When set, the net mode will be forced to Standalone when you hit Play in the editor
	UPROPERTY(EditDefaultsOnly, Category=PIE)
	bool ForceStandaloneNetMode = false;
#endif
};
