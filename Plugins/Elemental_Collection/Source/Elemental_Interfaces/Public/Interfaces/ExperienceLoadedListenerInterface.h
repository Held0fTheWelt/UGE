// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ExperienceLoadedListenerInterface.generated.h"

/**
 * Implemented by the game (e.g. GameInstance) to be notified when an experience has finished loading.
 * Allows the Experience system to notify Core/settings without depending on Core_Setup.
 */
UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UExperienceLoadedListenerInterface : public UInterface
{
	GENERATED_BODY()
};

class ELEMENTAL_INTERFACES_API IExperienceLoadedListenerInterface
{
	GENERATED_BODY()

public:
	/** Called when the current experience has finished loading. LoadedExperience is the experience definition (UObject). */
	UFUNCTION(BlueprintCallable, Category = "Experience")
	virtual void OnExperienceLoaded(const UObject* LoadedExperience) {}
};
