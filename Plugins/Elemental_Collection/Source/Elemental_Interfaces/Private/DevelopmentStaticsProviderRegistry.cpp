// Copyright Epic Games, Inc. All Rights Reserved.

#include "Interfaces/DevelopmentStaticsProviderInterface.h"

namespace DevelopmentStaticsProviderRegistry
{
	static IDevelopmentStaticsProvider* GProvider = nullptr;

	void Set(IDevelopmentStaticsProvider* Provider)
	{
		GProvider = Provider;
	}

	IDevelopmentStaticsProvider* Get()
	{
		return GProvider;
	}
}
