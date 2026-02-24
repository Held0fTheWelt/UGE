// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#include "Settings/CustomSettings/CoreSettingValueDiscrete_MobileFPSType.h"

#include "PerformanceStats/PlatformRenderingSettingsProvider.h"
#include "Settings/CoreSettingsLocal.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreSettingValueDiscrete_MobileFPSType)

#define LOCTEXT_NAMESPACE "CoreSettings"

UCoreSettingValueDiscrete_MobileFPSType::UCoreSettingValueDiscrete_MobileFPSType()
{
}

void UCoreSettingValueDiscrete_MobileFPSType::OnInitialized()
{
	Super::OnInitialized();

	const UCoreSettingsLocal* UserSettings = UCoreSettingsLocal::Get();
	const TArray<int32>* MobileLimits = nullptr;
	if (IPlatformRenderingSettingsProvider* Provider = CorePlatformRenderingSettings::GetInterfaceProvider())
	{
		MobileLimits = &Provider->GetMobileFrameRateLimits();
	}
	if (MobileLimits)
	{
	for (int32 TestLimit : *MobileLimits)
	{
		if (UCoreSettingsLocal::IsSupportedMobileFramePace(TestLimit))
		{
			FPSOptions.Add(TestLimit, MakeLimitString(TestLimit));
		}
	}
	}

	const int32 FirstFrameRateWithQualityLimit = UserSettings->GetFirstFrameRateWithQualityLimit();
	if (FirstFrameRateWithQualityLimit > 0)
	{
		SetWarningRichText(FText::Format(LOCTEXT("MobileFPSType_Note", "<strong>Note: Changing the framerate setting to {0} or higher might lower your Quality Presets.</>"), MakeLimitString(FirstFrameRateWithQualityLimit)));
	}
}

int32 UCoreSettingValueDiscrete_MobileFPSType::GetDefaultFPS() const
{
	return UCoreSettingsLocal::GetDefaultMobileFrameRate();
}

FText UCoreSettingValueDiscrete_MobileFPSType::MakeLimitString(int32 Number)
{
	return FText::Format(LOCTEXT("MobileFrameRateOption", "{0} FPS"), FText::AsNumber(Number));
}

void UCoreSettingValueDiscrete_MobileFPSType::StoreInitial()
{
	InitialValue = GetValue();
}

void UCoreSettingValueDiscrete_MobileFPSType::ResetToDefault()
{
	SetValue(GetDefaultFPS(), EGameSettingChangeReason::ResetToDefault);
}

void UCoreSettingValueDiscrete_MobileFPSType::RestoreToInitial()
{
	SetValue(InitialValue, EGameSettingChangeReason::RestoreToInitial);
}

void UCoreSettingValueDiscrete_MobileFPSType::SetDiscreteOptionByIndex(int32 Index)
{
	TArray<int32> FPSOptionsModes;
	FPSOptions.GenerateKeyArray(FPSOptionsModes);

	int32 NewMode = FPSOptionsModes.IsValidIndex(Index) ? FPSOptionsModes[Index] : GetDefaultFPS();

	SetValue(NewMode, EGameSettingChangeReason::Change);
}

int32 UCoreSettingValueDiscrete_MobileFPSType::GetDiscreteOptionIndex() const
{
	TArray<int32> FPSOptionsModes;
	FPSOptions.GenerateKeyArray(FPSOptionsModes);
	return FPSOptionsModes.IndexOfByKey(GetValue());
}

TArray<FText> UCoreSettingValueDiscrete_MobileFPSType::GetDiscreteOptions() const
{
	TArray<FText> Options;
	FPSOptions.GenerateValueArray(Options);

	return Options;
}

int32 UCoreSettingValueDiscrete_MobileFPSType::GetValue() const
{
	return UCoreSettingsLocal::Get()->GetDesiredMobileFrameRateLimit();
}

void UCoreSettingValueDiscrete_MobileFPSType::SetValue(int32 NewLimitFPS, EGameSettingChangeReason InReason)
{
	UCoreSettingsLocal::Get()->SetDesiredMobileFrameRateLimit(NewLimitFPS);

	NotifySettingChanged(InReason);
}

#undef LOCTEXT_NAMESPACE
