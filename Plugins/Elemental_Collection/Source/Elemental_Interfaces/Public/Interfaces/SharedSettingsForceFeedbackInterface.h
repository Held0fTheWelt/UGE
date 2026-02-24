// Copyright Epic Games, Inc. All Rights Reserved.
// Elemental_Interfaces: Shared settings object that exposes force feedback and change notification.
// Implemented by game-specific settings (e.g. Core); Controllers use only this interface.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SharedSettingsForceFeedbackInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class USharedSettingsForceFeedbackInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for shared settings objects that expose force feedback and change notification.
 * Used by Controllers to react to settings without depending on Core.
 */
class ELEMENTAL_INTERFACES_API ISharedSettingsForceFeedbackInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Settings")
	virtual bool GetForceFeedbackEnabled() const { return true; }

	/** Registers Listener to be called when any setting changes. FunctionName must be a UFUNCTION (e.g. OnSettingsChanged). */
	UFUNCTION(BlueprintCallable, Category = "Settings")
	virtual void AddSettingChangedListener(UObject* Listener, FName FunctionName) {}
};
