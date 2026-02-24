// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core_Replay.h"
#include "GameInstance/CoreGameInstanceDelegates.h"
#include "WarCollectionReplaySubsystem.h"

DEFINE_LOG_CATEGORY(LogCore_Replay);

void FCore_Replay::StartupModule()
{
	CoreGameInstanceDelegates::RegisterReplaySupportProvider(FDoesPlatformSupportReplaysDelegate::CreateLambda([]()
	{
		return UWarCollectionReplaySubsystem::DoesPlatformSupportReplays();
	}));
}

void FCore_Replay::ShutdownModule()
{
}

IMPLEMENT_MODULE(FCore_Replay, Core_Replay)
