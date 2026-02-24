// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PerformanceStats/PerformanceStatTypes.h"

/**
 * C++ interface for performance stat settings. Core_Performance's UCorePerformanceSettings implements this
 * and registers via SetPerformanceStatSettingsProvider(). Core_Setup uses GetPerformanceStatSettingsProvider()
 * so it does not depend on Core_Performance.
 */
class CORE_INTERFACES_API IPerformanceStatSettingsProvider
{
public:
	virtual ~IPerformanceStatSettingsProvider() = default;
	virtual const TArray<FCorePerformanceStatGroup>& GetUserFacingPerformanceStats() const = 0;
};

namespace CorePerformanceStatSettings
{
	/** Registers the provider. Call with (UObject*, static_cast<IPerformanceStatSettingsProvider*>(UObject)) from the implementing module. */
	CORE_INTERFACES_API void SetProvider(UObject* Provider, IPerformanceStatSettingsProvider* InterfaceProvider);
	CORE_INTERFACES_API UObject* GetProvider();
	/** Returns the interface so Core_Setup can use it without depending on Core_Performance. */
	CORE_INTERFACES_API IPerformanceStatSettingsProvider* GetInterfaceProvider();
}
