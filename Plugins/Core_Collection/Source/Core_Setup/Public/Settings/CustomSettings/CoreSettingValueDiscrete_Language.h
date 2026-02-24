// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#pragma once

#include "GameSettingValueDiscrete.h"

#include "CoreSettingValueDiscrete_Language.generated.h"

class FText;
class UObject;

UCLASS()
class CORE_SETUP_API UCoreSettingValueDiscrete_Language : public UGameSettingValueDiscrete
{
	GENERATED_BODY()
	
public:

	UCoreSettingValueDiscrete_Language();

	/** UGameSettingValue */
	virtual void StoreInitial() override;
	virtual void ResetToDefault() override;
	virtual void RestoreToInitial() override;

	/** UGameSettingValueDiscrete */
	virtual void SetDiscreteOptionByIndex(int32 Index) override;
	virtual int32 GetDiscreteOptionIndex() const override;
	virtual TArray<FText> GetDiscreteOptions() const override;

protected:
	/** UGameSettingValue */
	virtual void OnInitialized() override;
	virtual void OnApply() override;

protected:
	TArray<FString> AvailableCultureNames;
};
