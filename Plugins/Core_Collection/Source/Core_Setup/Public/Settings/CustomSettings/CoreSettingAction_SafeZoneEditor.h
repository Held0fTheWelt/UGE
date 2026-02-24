// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#pragma once

#include "GameSettingAction.h"
#include "GameSettingValueScalarDynamic.h"

#include "CoreSettingAction_SafeZoneEditor.generated.h"

class UGameSetting;
class UObject;

UCLASS()
class CORE_SETUP_API UCoreSettingValueScalarDynamic_SafeZoneValue : public UGameSettingValueScalarDynamic
{
	GENERATED_BODY()

public:
	virtual void ResetToDefault() override;
	virtual void RestoreToInitial() override;
};

UCLASS()
class CORE_SETUP_API UCoreSettingAction_SafeZoneEditor : public UGameSettingAction
{
	GENERATED_BODY()
	
public:
	UCoreSettingAction_SafeZoneEditor();
	virtual TArray<UGameSetting*> GetChildSettings() override;

private:
	UPROPERTY()
	TObjectPtr<UCoreSettingValueScalarDynamic_SafeZoneValue> SafeZoneValueSetting;
};
