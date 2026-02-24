// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManagerTypes.h"
#include "UObject/Interface.h"
#include "UObject/SoftObjectPath.h"
#include "ExperienceManagerInterface.generated.h"

class UExperienceDefinition;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnExperienceLoaded, const UExperienceDefinition* /*Experience*/);

/**
 * Interface for accessing the Experience Manager Component
 */
UINTERFACE(MinimalAPI, BlueprintType)
class UExperienceManagerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for accessing the Experience Manager Component
 */
class ELEMENTAL_INTERFACES_API IExperienceManagerInterface
{
	GENERATED_BODY()

public:
	/**
	 * Returns the Experience Manager Component if available
	 */
	virtual class UExperienceManagerComponent* GetExperienceManagerComponent() const = 0;

	/**
	 * Sets the current experience to load by primary asset id.
	 * Call this to request loading of an experience (e.g. from GameMode after resolving from options/world settings).
	 */
	virtual void SetCurrentExperience(FPrimaryAssetId ExperienceId) = 0;

	/**
	 * Ensures the delegate is called once the experience has been loaded
	 * If the experience has already loaded, calls the delegate immediately
	 */
	virtual void CallOrRegister_OnExperienceLoaded(FOnExperienceLoaded::FDelegate&& Delegate) = 0;

	/**
	 * Returns true if the experience is fully loaded
	 */
	virtual bool IsExperienceLoaded() const = 0;

	/**
	 * Returns the current experience if it is fully loaded, nullptr otherwise
	 */
	virtual const UExperienceDefinition* GetCurrentExperience() const = 0;
	virtual void CallOrRegister_OnExperienceLoaded_LowPriority(FOnExperienceLoaded::FDelegate&& Delegate) = 0;

	/**
	 * Returns the soft path to the default pawn data from the current experience, if loaded and set.
	 * Enables callers (e.g. GameMode) to resolve pawn data without depending on the concrete experience type.
	 */
	virtual FSoftObjectPath GetDefaultPawnDataPathFromCurrentExperience() const = 0;

	/** Whether to destroy player state when the player deactivates (disconnects). Default true. Override to keep around (e.g. long-running servers). */
	virtual bool ShouldDestroyDeactivatedPlayerState() const { return true; }
};
