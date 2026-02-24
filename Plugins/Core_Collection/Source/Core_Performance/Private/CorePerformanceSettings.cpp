// Copyright Epic Games, Inc. All Rights Reserved.
// Core_Collection

#include "CorePerformanceSettings.h"

#include "Engine/PlatformSettingsManager.h"
#include "Misc/EnumRange.h"
#include "CorePerformanceStatTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CorePerformanceSettings)

UCorePlatformSpecificRenderingSettings::UCorePlatformSpecificRenderingSettings()
{
	MobileFrameRateLimits.Append({ 20, 30, 45, 60, 90, 120 });
}

const UCorePlatformSpecificRenderingSettings* UCorePlatformSpecificRenderingSettings::Get()
{
	UCorePlatformSpecificRenderingSettings* Result = UPlatformSettingsManager::Get().GetSettingsForPlatform<ThisClass>();
	check(Result);
	return Result;
}

UCorePlatformSpecificRenderingSettings* UCorePlatformSpecificRenderingSettings::GetMutable()
{
	return const_cast<UCorePlatformSpecificRenderingSettings*>(Get());
}

UCorePerformanceSettings::UCorePerformanceSettings()
{
	PerPlatformSettings.Initialize(UCorePlatformSpecificRenderingSettings::StaticClass());
	CategoryName = TEXT("Game");
	DesktopFrameRateLimits.Append({ 30, 60, 120, 144, 160, 165, 180, 200, 240, 360 });
	FCorePerformanceStatGroup& StatGroup = UserFacingPerformanceStats.AddDefaulted_GetRef();
	for (ECoreDisplayablePerformanceStat PerfStat : TEnumRange<ECoreDisplayablePerformanceStat>())
	{
		StatGroup.AllowedStats.Add(PerfStat);
	}
}
