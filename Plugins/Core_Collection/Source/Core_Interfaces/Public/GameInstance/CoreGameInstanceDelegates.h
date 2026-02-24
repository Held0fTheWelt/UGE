// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/** Delegate for experience loaded notification. Core_Setup registers here; Core_System's game instance broadcasts. */
DECLARE_DELEGATE_OneParam(FOnExperienceLoadedDelegate, const UObject* /*LoadedExperience*/);

/** Delegate for replay support query. Core_Replay registers here; Core_System's game instance queries. */
DECLARE_DELEGATE_RetVal(bool, FDoesPlatformSupportReplaysDelegate);

/**
 * Central registry for game instance-related delegates so Core_Setup and Core_Replay
 * can register without depending on Core_System. Core_System's UGameInstance_Core
 * invokes/queries these.
 */
namespace CoreGameInstanceDelegates
{
	CORE_INTERFACES_API void RegisterExperienceLoadedListener(FOnExperienceLoadedDelegate Delegate);
	CORE_INTERFACES_API void BroadcastExperienceLoaded(const UObject* LoadedExperience);

	CORE_INTERFACES_API void RegisterReplaySupportProvider(FDoesPlatformSupportReplaysDelegate Delegate);
	CORE_INTERFACES_API bool DoesPlatformSupportReplays();
}
