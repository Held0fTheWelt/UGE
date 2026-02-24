// Copyright Epic Games, Inc. All Rights Reserved.

#include "PerformanceStats/PerformanceStatSettingsProvider.h"

namespace CorePerformanceStatSettings
{
	static TWeakObjectPtr<UObject> GProvider;
	static IPerformanceStatSettingsProvider* GInterfaceProvider = nullptr;

	void SetProvider(UObject* Provider, IPerformanceStatSettingsProvider* InterfaceProvider)
	{
		GProvider = Provider;
		GInterfaceProvider = InterfaceProvider;
	}

	UObject* GetProvider()
	{
		return GProvider.Get();
	}

	IPerformanceStatSettingsProvider* GetInterfaceProvider()
	{
		return GProvider.IsValid() ? GInterfaceProvider : nullptr;
	}
}
