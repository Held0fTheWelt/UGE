// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ReplaySupportProviderInterface.generated.h"

/**
 * Implemented by the game (e.g. GameInstance or a subsystem) to query replay support.
 * Allows the Experience system to check replay support without depending on Core_Replay.
 */
UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UReplaySupportProviderInterface : public UInterface
{
	GENERATED_BODY()
};

class ELEMENTAL_INTERFACES_API IReplaySupportProviderInterface
{
	GENERATED_BODY()

public:
	/** Returns true if the current platform supports replays (e.g. for session creation options). */
	UFUNCTION(BlueprintCallable, Category = "Replay")
	virtual bool DoesPlatformSupportReplays() const { return false; }
};
