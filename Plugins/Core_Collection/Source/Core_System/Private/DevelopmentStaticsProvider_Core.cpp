// Copyright Epic Games, Inc. All Rights Reserved.

#include "DevelopmentStaticsProvider_Core.h"
#include "Interfaces/DevelopmentStaticsProviderInterface.h"
#include "Libs/DevelopmentStatics_Core.h"
#include "Engine/World.h"

namespace
{
	struct FDevelopmentStaticsProvider_Core : IDevelopmentStaticsProvider
	{
		virtual UWorld* GetPlayInEditorAuthorityWorld() override
		{
			return UDevelopmentStatics_Core::FindPlayInEditorAuthorityWorld();
		}

		virtual UClass* FindClassByShortName(const FString& SearchToken, UClass* DesiredBaseClass, bool bLogFailures) override
		{
			return UDevelopmentStatics_Core::FindClassByShortName(SearchToken, DesiredBaseClass, bLogFailures);
		}
	};

	static FDevelopmentStaticsProvider_Core GDevelopmentStaticsProvider_Core;
}

IDevelopmentStaticsProvider* Core_SystemDevelopmentStatics::GetProvider()
{
	return &GDevelopmentStaticsProvider_Core;
}
