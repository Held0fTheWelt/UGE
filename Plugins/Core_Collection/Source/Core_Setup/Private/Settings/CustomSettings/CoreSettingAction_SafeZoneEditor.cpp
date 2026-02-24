// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#include "Settings/CustomSettings/CoreSettingAction_SafeZoneEditor.h"

#include "DataSource/GameSettingDataSource.h"
#include "Settings/CoreGameSettingRegistry.h"
#include "Settings/CoreSettingsLocal.h"
#include "Widgets/Layout/SSafeZone.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreSettingAction_SafeZoneEditor)

class UGameSetting;

#define LOCTEXT_NAMESPACE "CoreSettings"

UCoreSettingAction_SafeZoneEditor::UCoreSettingAction_SafeZoneEditor()
{
	SafeZoneValueSetting = NewObject<UCoreSettingValueScalarDynamic_SafeZoneValue>();
	SafeZoneValueSetting->SetDevName(TEXT("SafeZoneValue"));
	SafeZoneValueSetting->SetDisplayName(LOCTEXT("SafeZoneValue_Name", "Safe Zone Value"));
	SafeZoneValueSetting->SetDescriptionRichText(LOCTEXT("SafeZoneValue_Description", "The safezone area percentage."));
	SafeZoneValueSetting->SetDefaultValue(0.0f);
	SafeZoneValueSetting->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetSafeZone));
	SafeZoneValueSetting->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetSafeZone));
	SafeZoneValueSetting->SetDisplayFormat([](double SourceValue, double NormalizedValue){ return FText::AsNumber(SourceValue); });
	SafeZoneValueSetting->SetSettingParent(this);
}

TArray<UGameSetting*> UCoreSettingAction_SafeZoneEditor::GetChildSettings()
{
	return { SafeZoneValueSetting };
}

void UCoreSettingValueScalarDynamic_SafeZoneValue::ResetToDefault()
{
	Super::ResetToDefault();
	SSafeZone::SetGlobalSafeZoneScale(TOptional<float>(DefaultValue.Get(0.0f)));
}

void UCoreSettingValueScalarDynamic_SafeZoneValue::RestoreToInitial()
{
	Super::RestoreToInitial();
	SSafeZone::SetGlobalSafeZoneScale(TOptional<float>(InitialValue));
}

#undef LOCTEXT_NAMESPACE
