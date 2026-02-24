// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameInstance/CoreGameInstanceDelegates.h"

namespace CoreGameInstanceDelegates
{
	static FOnExperienceLoadedDelegate GExperienceLoadedDelegate;
	static FDoesPlatformSupportReplaysDelegate GReplaySupportDelegate;

	void RegisterExperienceLoadedListener(FOnExperienceLoadedDelegate Delegate)
	{
		GExperienceLoadedDelegate = MoveTemp(Delegate);
	}

	void BroadcastExperienceLoaded(const UObject* LoadedExperience)
	{
		if (GExperienceLoadedDelegate.IsBound())
		{
			GExperienceLoadedDelegate.Execute(LoadedExperience);
		}
	}

	void RegisterReplaySupportProvider(FDoesPlatformSupportReplaysDelegate Delegate)
	{
		GReplaySupportDelegate = MoveTemp(Delegate);
	}

	bool DoesPlatformSupportReplays()
	{
		return GReplaySupportDelegate.IsBound() ? GReplaySupportDelegate.Execute() : false;
	}
}
