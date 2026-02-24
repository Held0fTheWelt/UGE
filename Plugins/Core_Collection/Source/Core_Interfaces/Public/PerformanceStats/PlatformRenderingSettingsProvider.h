// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * C++ interface for platform rendering settings (e.g. mobile frame rate limits).
 * Core_Performance's UCorePlatformSpecificRenderingSettings implements this and registers.
 * Core_Setup uses GetProvider() so it does not depend on Core_Performance.
 */
class CORE_INTERFACES_API IPlatformRenderingSettingsProvider
{
public:
	virtual ~IPlatformRenderingSettingsProvider() = default;
	virtual const TArray<int32>& GetMobileFrameRateLimits() const = 0;
};

namespace CorePlatformRenderingSettings
{
	/** Registers the provider. Call with (UObject*, static_cast<IPlatformRenderingSettingsProvider*>(UObject)) from the implementing module. */
	CORE_INTERFACES_API void SetProvider(UObject* Provider, IPlatformRenderingSettingsProvider* InterfaceProvider);
	CORE_INTERFACES_API UObject* GetProvider();
	/** Returns the interface pointer so Core_Setup can use it without Cast<> (Cast does not work for non-UINTERFACE). */
	CORE_INTERFACES_API IPlatformRenderingSettingsProvider* GetInterfaceProvider();
}
