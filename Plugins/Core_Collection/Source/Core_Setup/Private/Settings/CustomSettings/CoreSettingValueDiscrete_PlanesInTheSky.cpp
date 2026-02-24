// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core – PlanesInTheSky Checkbox

#include "Settings/CustomSettings/CoreSettingValueDiscrete_PlanesInTheSky.h"

#include "Settings/CoreSettingsLocal.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreSettingValueDiscrete_PlanesInTheSky)

#define LOCTEXT_NAMESPACE "CoreSettings"

UCoreSettingValueDiscrete_PlanesInTheSky::UCoreSettingValueDiscrete_PlanesInTheSky()
{
	SetDevName(TEXT("PlanesInTheSkyEnabled"));
}

void UCoreSettingValueDiscrete_PlanesInTheSky::OnInitialized()
{
	Super::OnInitialized();

	Options.Add(LOCTEXT("PlanesInTheSky_Enabled", "Aktiviert"));
	Options.Add(LOCTEXT("PlanesInTheSky_Disabled", "Deaktiviert"));
}

void UCoreSettingValueDiscrete_PlanesInTheSky::StoreInitial()
{
	const UCoreSettingsLocal* Settings = UCoreSettingsLocal::Get();
	bInitialEnabled = Settings ? Settings->IsPlanesInTheSkyEnabled() : true;
}

void UCoreSettingValueDiscrete_PlanesInTheSky::ResetToDefault()
{
	if (UCoreSettingsLocal* Settings = UCoreSettingsLocal::Get())
	{
		Settings->SetPlanesInTheSkyEnabled(true);
	}
	NotifySettingChanged(EGameSettingChangeReason::ResetToDefault);
}

void UCoreSettingValueDiscrete_PlanesInTheSky::RestoreToInitial()
{
	if (UCoreSettingsLocal* Settings = UCoreSettingsLocal::Get())
	{
		Settings->SetPlanesInTheSkyEnabled(bInitialEnabled);
	}
	NotifySettingChanged(EGameSettingChangeReason::RestoreToInitial);
}

void UCoreSettingValueDiscrete_PlanesInTheSky::SetDiscreteOptionByIndex(int32 Index)
{
	// Index 0 = Aktiviert, Index 1 = Deaktiviert
	if (UCoreSettingsLocal* Settings = UCoreSettingsLocal::Get())
	{
		Settings->SetPlanesInTheSkyEnabled(Index == 0);
	}
	NotifySettingChanged(EGameSettingChangeReason::Change);
}

int32 UCoreSettingValueDiscrete_PlanesInTheSky::GetDiscreteOptionIndex() const
{
	const UCoreSettingsLocal* Settings = UCoreSettingsLocal::Get();
	return (Settings && Settings->IsPlanesInTheSkyEnabled()) ? 0 : 1;
}

TArray<FText> UCoreSettingValueDiscrete_PlanesInTheSky::GetDiscreteOptions() const
{
	return Options;
}

#undef LOCTEXT_NAMESPACE
