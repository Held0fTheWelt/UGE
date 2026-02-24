// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "Core_Performance.h"
#include "CorePerformanceSettings.h"
#include "PerformanceStats/PerformanceStatSettingsProvider.h"
#include "PerformanceStats/PlatformRenderingSettingsProvider.h"

DEFINE_LOG_CATEGORY(LogCore_Performance);

class FCore_PerformanceModule : public IModuleInterface
{
public:
	virtual void StartupModule() override
	{
		{
			UObject* Obj = GetMutableDefault<UCorePerformanceSettings>();
			CorePerformanceStatSettings::SetProvider(Obj, static_cast<IPerformanceStatSettingsProvider*>(Obj));
		}
		{
			UObject* Obj = UCorePlatformSpecificRenderingSettings::GetMutable();
			CorePlatformRenderingSettings::SetProvider(Obj, static_cast<IPlatformRenderingSettingsProvider*>(Obj));
		}
	}
};

IMPLEMENT_MODULE(FCore_PerformanceModule, Core_Performance)
