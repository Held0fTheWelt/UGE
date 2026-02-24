// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core – PlanesInTheSky Checkbox

#pragma once

#include "GameSettingValueDiscrete.h"

#include "CoreSettingValueDiscrete_PlanesInTheSky.generated.h"

class UObject;

UCLASS()
class CORE_SETUP_API UCoreSettingValueDiscrete_PlanesInTheSky : public UGameSettingValueDiscrete
{
	GENERATED_BODY()

public:
	UCoreSettingValueDiscrete_PlanesInTheSky();

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

	TArray<FText> Options;
	bool bInitialEnabled = true;
};
