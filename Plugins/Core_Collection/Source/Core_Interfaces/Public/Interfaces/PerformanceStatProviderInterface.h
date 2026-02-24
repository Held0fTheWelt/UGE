// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PerformanceStatProviderInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType, NotBlueprintable)
class UPerformanceStatProviderInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for game state (or other) objects that provide server FPS to the performance stat subsystem.
 * Used to break the circular dependency between Core_Classes and Core_Performance.
 */
class CORE_INTERFACES_API IPerformanceStatProviderInterface
{
	GENERATED_BODY()

public:
	/** Returns the server FPS (replicated to clients). */
	UFUNCTION(BlueprintCallable, Category = "Performance")
	virtual float GetServerFPS() const { return 0.0f; }
};
