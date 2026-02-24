// Copyright Epic Games, Inc. All Rights Reserved.

#include "PerformanceStats/PlatformRenderingSettingsProvider.h"

namespace CorePlatformRenderingSettings
{
	static TWeakObjectPtr<UObject> GProvider;
	static IPlatformRenderingSettingsProvider* GInterfaceProvider = nullptr;

	void SetProvider(UObject* Provider, IPlatformRenderingSettingsProvider* InterfaceProvider)
	{
		GProvider = Provider;
		GInterfaceProvider = InterfaceProvider;
	}

	UObject* GetProvider()
	{
		return GProvider.Get();
	}

	IPlatformRenderingSettingsProvider* GetInterfaceProvider()
	{
		// Only return if UObject is still valid to avoid dangling interface pointer
		return GProvider.IsValid() ? GInterfaceProvider : nullptr;
	}
}
