// Copyright Epic Games, Inc. All Rights Reserved.
// Elemental_Interfaces: Local player that provides shared settings (e.g. for force feedback).
// Implemented by game-specific local player (e.g. Core); Controllers use only this interface.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LocalPlayerSharedSettingsProviderInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class ULocalPlayerSharedSettingsProviderInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for local player (ULocalPlayer) that provides access to shared settings.
 * Used by Controllers to subscribe to settings changes without depending on Core.
 */
class ELEMENTAL_INTERFACES_API ILocalPlayerSharedSettingsProviderInterface
{
	GENERATED_BODY()

public:
	/** Returns the shared settings object (e.g. for force feedback, replay). May implement ISharedSettingsForceFeedbackInterface. */
	UFUNCTION(BlueprintCallable, Category = "LocalPlayer")
	virtual UObject* GetSharedSettings() const { return nullptr; }
};
