// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#pragma once

#include "GameSettingValueDiscrete.h"
#include "PerformanceStats/PerformanceStatTypes.h"

#include "CoreSettingValueDiscrete_PerfStat.generated.h"

class UObject;

UCLASS()
class CORE_SETUP_API UCoreSettingValueDiscrete_PerfStat : public UGameSettingValueDiscrete
{
	GENERATED_BODY()
	
public:

	UCoreSettingValueDiscrete_PerfStat();

	void SetStat(ECoreDisplayablePerformanceStat InStat);

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
	
	void AddMode(FText&& Label, ECoreStatDisplayMode Mode);
	
protected:
	TArray<FText> Options;
	TArray<ECoreStatDisplayMode> DisplayModes;

	ECoreDisplayablePerformanceStat StatToDisplay;
	ECoreStatDisplayMode InitialMode;
};
