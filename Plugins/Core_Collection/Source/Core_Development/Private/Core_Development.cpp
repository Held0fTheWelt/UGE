#include "Core_Development.h"
#include "HAL/IConsoleManager.h"

DEFINE_LOG_CATEGORY(Core_Development);

namespace
{
	static bool GShouldAlwaysPlayForceFeedback = false;
	static FAutoConsoleVariableRef CVarShouldAlwaysPlayForceFeedback(
		TEXT("Core.Settings.ShouldAlwaysPlayForceFeedback"),
		GShouldAlwaysPlayForceFeedback,
		TEXT("Should force feedback effects be played even when the last input device was not a gamepad?"),
		ECVF_Default);

	static bool GApplyFrameRateSettingsInPIE = false;
	static FAutoConsoleVariableRef CVarApplyFrameRateSettingsInPIE(
		TEXT("Core.Settings.ApplyFrameRateSettingsInPIE"),
		GApplyFrameRateSettingsInPIE,
		TEXT("Apply desktop-style frame rate settings in PIE?"),
		ECVF_Default);

	static bool GApplyFrontEndPerformanceOptionsInPIE = false;
	static FAutoConsoleVariableRef CVarApplyFrontEndPerformanceOptionsInPIE(
		TEXT("Core.Settings.ApplyFrontEndPerformanceOptionsInPIE"),
		GApplyFrontEndPerformanceOptionsInPIE,
		TEXT("Apply front-end specific performance options in PIE?"),
		ECVF_Default);

	static bool GApplyDeviceProfilesInPIE = false;
	static FAutoConsoleVariableRef CVarApplyDeviceProfilesInPIE(
		TEXT("Core.Settings.ApplyDeviceProfilesInPIE"),
		GApplyDeviceProfilesInPIE,
		TEXT("Apply experience/platform emulated device profiles in PIE?"),
		ECVF_Default);

	// WeaponDebugSettings (GameFeature_WeaponSystem) – CVars must exist before that module loads
	static float GWeaponDrawBulletTraceDuration = 0.0f;
	static FAutoConsoleVariableRef CVarWeaponDrawBulletTraceDuration(
		TEXT("WarCollection.Weapon.DrawBulletTraceDuration"),
		GWeaponDrawBulletTraceDuration,
		TEXT("Duration to draw bullet traces (seconds)"),
		ECVF_Default);

	static float GWeaponDrawBulletHitDuration = 0.0f;
	static FAutoConsoleVariableRef CVarWeaponDrawBulletHitDuration(
		TEXT("WarCollection.Weapon.DrawBulletHitDuration"),
		GWeaponDrawBulletHitDuration,
		TEXT("Duration to draw bullet hit markers (seconds)"),
		ECVF_Default);

	static float GWeaponDrawBulletHitRadius = 0.0f;
	static FAutoConsoleVariableRef CVarWeaponDrawBulletHitRadius(
		TEXT("WarCollection.Weapon.DrawBulletHitRadius"),
		GWeaponDrawBulletHitRadius,
		TEXT("Radius of bullet hit debug draw (cm)"),
		ECVF_Default);
}

#define LOCTEXT_NAMESPACE "FCore_Development"

void FCore_Development::StartupModule()
{
	UE_LOG(Core_Development, Verbose, TEXT("Core_Development module has been loaded"));
}

void FCore_Development::ShutdownModule()
{
	UE_LOG(Core_Development, Verbose, TEXT("Core_Development module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCore_Development, Core_Development)