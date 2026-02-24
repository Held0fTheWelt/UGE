// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#include "Executions/CoreHealExecution.h"

#include "Attributes/CoreCombatSet.h"
#include "Attributes/CoreHealthSet.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreHealExecution)

struct FHealStatics
{
	FGameplayEffectAttributeCaptureDefinition BaseHealDef;
	FGameplayEffectAttributeCaptureDefinition HealingDef;

	FHealStatics()
	{
		// Capture the Heal from the source
		BaseHealDef = FGameplayEffectAttributeCaptureDefinition(UCoreCombatSet::GetBaseHealAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);

		// Also capture the source's incoming healing, which is normally passed in via the execution parameters
		HealingDef = FGameplayEffectAttributeCaptureDefinition(UCoreHealthSet::GetHealingAttribute(), EGameplayEffectAttributeCaptureSource::Source, false);
	}
};

static FHealStatics& HealStatics()
{
	static FHealStatics Statics;
	return Statics;
}

UCoreHealExecution::UCoreHealExecution()
{
	RelevantAttributesToCapture.Add(HealStatics().BaseHealDef);
	RelevantAttributesToCapture.Add(HealStatics().HealingDef);
}

void UCoreHealExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float BaseHeal = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealStatics().BaseHealDef, EvaluateParameters, BaseHeal);
	if (BaseHeal == 0.0f)
	{
		// If base heal is 0, there is no healing to add, so leave the Healing at 0.
		return;
	}

	float Healing = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealStatics().HealingDef, EvaluateParameters, Healing);

	const float FinalHealing = BaseHeal + Healing;

	if (FinalHealing > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UCoreHealthSet::GetHealingAttribute(), EGameplayModOp::Additive, FinalHealing));
	}
}
