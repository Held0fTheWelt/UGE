// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#include "Settings/CustomSettings/CoreSettingValueDiscrete_PerfStat.h"

#include "CommonUIVisibilitySubsystem.h"
#include "PerformanceStats/PerformanceStatSettingsProvider.h"
#include "PerformanceStats/PerformanceStatTypes.h"
#include "Settings/CoreSettingsLocal.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreSettingValueDiscrete_PerfStat)

class ULocalPlayer;

#define LOCTEXT_NAMESPACE "CoreSettings"

//////////////////////////////////////////////////////////////////////

class FGameSettingEditCondition_PerfStatAllowed : public FGameSettingEditCondition
{
public:
	static TSharedRef<FGameSettingEditCondition_PerfStatAllowed> Get(ECoreDisplayablePerformanceStat Stat)
	{
		return MakeShared<FGameSettingEditCondition_PerfStatAllowed>(Stat);
	}

	FGameSettingEditCondition_PerfStatAllowed(ECoreDisplayablePerformanceStat Stat)
		: AssociatedStat(Stat)
	{
	}

	//~FGameSettingEditCondition interface
	virtual void GatherEditState(const ULocalPlayer* InLocalPlayer, FGameSettingEditableState& InOutEditState) const override
	{
		const FGameplayTagContainer& VisibilityTags = UCommonUIVisibilitySubsystem::GetChecked(InLocalPlayer)->GetVisibilityTags();

		bool bCanShowStat = false;
		if (const IPerformanceStatSettingsProvider* SettingsProvider = CorePerformanceStatSettings::GetInterfaceProvider())
		{
		for (const FCorePerformanceStatGroup& Group : SettingsProvider->GetUserFacingPerformanceStats())
		{
			if (Group.AllowedStats.Contains(AssociatedStat))
			{
				const bool bShowGroup = (Group.VisibilityQuery.IsEmpty() || Group.VisibilityQuery.Matches(VisibilityTags));
				if (bShowGroup)
				{
					bCanShowStat = true;
					break;
				}
			}
		}
		}

		if (!bCanShowStat)
		{
			InOutEditState.Hide(TEXT("Stat is not listed in performance settings or is suppressed by current platform traits"));
		}
	}
	//~End of FGameSettingEditCondition interface

private:
	ECoreDisplayablePerformanceStat AssociatedStat;
};

//////////////////////////////////////////////////////////////////////

UCoreSettingValueDiscrete_PerfStat::UCoreSettingValueDiscrete_PerfStat()
{
}

void UCoreSettingValueDiscrete_PerfStat::SetStat(ECoreDisplayablePerformanceStat InStat)
{
	StatToDisplay = InStat;
	SetDevName(FName(*FString::Printf(TEXT("PerfStat_%d"), (int32)StatToDisplay)));
	AddEditCondition(FGameSettingEditCondition_PerfStatAllowed::Get(StatToDisplay));
}

void UCoreSettingValueDiscrete_PerfStat::AddMode(FText&& Label, ECoreStatDisplayMode Mode)
{
	Options.Emplace(MoveTemp(Label));
	DisplayModes.Add(Mode);
}

void UCoreSettingValueDiscrete_PerfStat::OnInitialized()
{
	Super::OnInitialized();

	AddMode(LOCTEXT("PerfStatDisplayMode_None", "None"), ECoreStatDisplayMode::Hidden);
	AddMode(LOCTEXT("PerfStatDisplayMode_TextOnly", "Text Only"), ECoreStatDisplayMode::TextOnly);
	AddMode(LOCTEXT("PerfStatDisplayMode_GraphOnly", "Graph Only"), ECoreStatDisplayMode::GraphOnly);
	AddMode(LOCTEXT("PerfStatDisplayMode_TextAndGraph", "Text and Graph"), ECoreStatDisplayMode::TextAndGraph);
}

void UCoreSettingValueDiscrete_PerfStat::StoreInitial()
{
	const UCoreSettingsLocal* Settings = UCoreSettingsLocal::Get();
	InitialMode = Settings->GetPerfStatDisplayState(StatToDisplay);
}

void UCoreSettingValueDiscrete_PerfStat::ResetToDefault()
{
	UCoreSettingsLocal* Settings = UCoreSettingsLocal::Get();
	Settings->SetPerfStatDisplayState(StatToDisplay, ECoreStatDisplayMode::Hidden);
	NotifySettingChanged(EGameSettingChangeReason::ResetToDefault);
}

void UCoreSettingValueDiscrete_PerfStat::RestoreToInitial()
{
	UCoreSettingsLocal* Settings = UCoreSettingsLocal::Get();
	Settings->SetPerfStatDisplayState(StatToDisplay, InitialMode);
	NotifySettingChanged(EGameSettingChangeReason::RestoreToInitial);
}

void UCoreSettingValueDiscrete_PerfStat::SetDiscreteOptionByIndex(int32 Index)
{
	if (DisplayModes.IsValidIndex(Index))
	{
		const ECoreStatDisplayMode DisplayMode = DisplayModes[Index];

		UCoreSettingsLocal* Settings = UCoreSettingsLocal::Get();
		Settings->SetPerfStatDisplayState(StatToDisplay, DisplayMode);
	}
	NotifySettingChanged(EGameSettingChangeReason::Change);
}

int32 UCoreSettingValueDiscrete_PerfStat::GetDiscreteOptionIndex() const
{
	const UCoreSettingsLocal* Settings = UCoreSettingsLocal::Get();
	return DisplayModes.Find(Settings->GetPerfStatDisplayState(StatToDisplay));
}

TArray<FText> UCoreSettingValueDiscrete_PerfStat::GetDiscreteOptions() const
{
	return Options;
}

#undef LOCTEXT_NAMESPACE
