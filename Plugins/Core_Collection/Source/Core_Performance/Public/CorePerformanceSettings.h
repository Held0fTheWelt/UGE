// Copyright Epic Games, Inc. All Rights Reserved.
// Core_Collection

#pragma once

#include "Engine/DeveloperSettingsBackedByCVars.h"
#include "Engine/PlatformSettings.h"
#include "PerformanceStats/PerformanceStatTypes.h"
#include "PerformanceStats/PerformanceStatSettingsProvider.h"
#include "PerformanceStats/PlatformRenderingSettingsProvider.h"

#include "CorePerformanceSettings.generated.h"

class UObject;

USTRUCT()
struct FCoreQualityDeviceProfileVariant
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText DisplayName;

	UPROPERTY(EditAnywhere)
	FString DeviceProfileSuffix;

	UPROPERTY(EditAnywhere)
	int32 MinRefreshRate = 0;
};

UENUM()
enum class ECoreFramePacingMode : uint8
{
	DesktopStyle,
	ConsoleStyle,
	MobileStyle
};

UCLASS(config=Game, defaultconfig)
class CORE_PERFORMANCE_API UCorePlatformSpecificRenderingSettings : public UPlatformSettings, public IPlatformRenderingSettingsProvider
{
	GENERATED_BODY()

public:
	UCorePlatformSpecificRenderingSettings();

	static const UCorePlatformSpecificRenderingSettings* Get();
	/** For registration with Core_Interfaces provider (Core_Setup uses without depending on Core_Performance). */
	static UCorePlatformSpecificRenderingSettings* GetMutable();

	//~IPlatformRenderingSettingsProvider
	virtual const TArray<int32>& GetMobileFrameRateLimits() const override { return MobileFrameRateLimits; }

public:
	UPROPERTY(EditAnywhere, Config, Category=DeviceProfiles)
	FString DefaultDeviceProfileSuffix;

	UPROPERTY(EditAnywhere, Config, Category=DeviceProfiles)
	TArray<FCoreQualityDeviceProfileVariant> UserFacingDeviceProfileOptions;

	UPROPERTY(EditAnywhere, Config, Category=VideoSettings)
	bool bSupportsGranularVideoQualitySettings = true;

	UPROPERTY(EditAnywhere, Config, Category=VideoSettings)
	bool bSupportsAutomaticVideoQualityBenchmark = true;

	UPROPERTY(EditAnywhere, Config, Category=VideoSettings)
	ECoreFramePacingMode FramePacingMode = ECoreFramePacingMode::DesktopStyle;

	UPROPERTY(EditAnywhere, Config, Category=VideoSettings, meta=(EditCondition="FramePacingMode==ECoreFramePacingMode::MobileStyle", ForceUnits=Hz))
	TArray<int32> MobileFrameRateLimits;
};

//////////////////////////////////////////////////////////////////////

UCLASS(config=Game, defaultconfig, meta=(DisplayName="Core Performance Settings"))
class CORE_PERFORMANCE_API UCorePerformanceSettings : public UDeveloperSettingsBackedByCVars, public IPerformanceStatSettingsProvider
{
	GENERATED_BODY()

public:
	UCorePerformanceSettings();

	//~IPerformanceStatSettingsProvider
	virtual const TArray<FCorePerformanceStatGroup>& GetUserFacingPerformanceStats() const override { return UserFacingPerformanceStats; }

private:
	UPROPERTY(EditAnywhere, Category = "PlatformSpecific")
	FPerPlatformSettings PerPlatformSettings;

public:
	UPROPERTY(EditAnywhere, Config, Category=Performance, meta=(ForceUnits=Hz))
	TArray<int32> DesktopFrameRateLimits;

	UPROPERTY(EditAnywhere, Config, Category=Stats)
	TArray<FCorePerformanceStatGroup> UserFacingPerformanceStats;
};
