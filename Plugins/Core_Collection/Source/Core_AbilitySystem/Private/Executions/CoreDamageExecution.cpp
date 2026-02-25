// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#include "Executions/CoreDamageExecution.h"

#include "Attributes/CoreCombatSet.h"
#include "Attributes/CoreHealthSet.h"
#include "CoreGameplayEffectContext.h"
#include "CoreAbilitySourceInterface.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemComponent.h"
#include "Engine/World.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreDamageExecution)

struct FDamageStatics
{
	FGameplayEffectAttributeCaptureDefinition BaseDamageDef;

	FDamageStatics()
	{
		// Capture the Damage from the source
		BaseDamageDef = FGameplayEffectAttributeCaptureDefinition(UCoreCombatSet::GetBaseDamageAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	}
};

static FDamageStatics& DamageStatics()
{
	static FDamageStatics Statics;
	return Statics;
}

UCoreDamageExecution::UCoreDamageExecution()
{
	RelevantAttributesToCapture.Add(DamageStatics().BaseDamageDef);
}

void UCoreDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
#if WITH_SERVER_CODE
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FCoreGameplayEffectContext* TypedContext = FCoreGameplayEffectContext::ExtractEffectContext(Spec.GetContext());
	
	if (!TypedContext)
	{
		// Fallback to standard execution if context is not Core type
		const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
		const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

		FAggregatorEvaluateParameters EvaluateParameters;
		EvaluateParameters.SourceTags = SourceTags;
		EvaluateParameters.TargetTags = TargetTags;

		float BaseDamage = 0.0f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BaseDamageDef, EvaluateParameters, BaseDamage);

		if (BaseDamage > 0.0f)
		{
			OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UCoreHealthSet::GetDamageAttribute(), EGameplayModOp::Additive, BaseDamage));
		}
		return;
	}

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float BaseDamage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BaseDamageDef, EvaluateParameters, BaseDamage);

	const AActor* EffectCauser = TypedContext->GetEffectCauser();
	const FHitResult* HitActorResult = TypedContext->GetHitResult();

	AActor* HitActor = nullptr;
	FVector ImpactLocation = FVector::ZeroVector;

	// Calculation of hit actor, surface, zone, and distance all rely on whether the calculation has a hit result or not.
	if (HitActorResult)
	{
		const FHitResult& CurHitResult = *HitActorResult;
		HitActor = CurHitResult.HitObjectHandle.FetchActor();
		if (HitActor)
		{
			ImpactLocation = CurHitResult.ImpactPoint;
		}
	}

	// Handle case of no hit result or hit result not actually returning an actor
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	if (!HitActor)
	{
		HitActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor_Direct() : nullptr;
		if (HitActor)
		{
			ImpactLocation = HitActor->GetActorLocation();
		}
	}

	// Apply rules for team damage/self damage/etc...
	// TODO: Integrate with Team-System when available
	float DamageInteractionAllowedMultiplier = 1.0f;
	// if (HitActor)
	// {
	// 	// TODO: Add Team-System integration
	// 	// UCoreTeamSubsystem* TeamSubsystem = HitActor->GetWorld()->GetSubsystem<UCoreTeamSubsystem>();
	// 	// if (ensure(TeamSubsystem))
	// 	// {
	// 	// 	DamageInteractionAllowedMultiplier = TeamSubsystem->CanCauseDamage(EffectCauser, HitActor) ? 1.0 : 0.0;
	// 	// }
	// }

	// Determine distance
	double Distance = WORLD_MAX;

	if (TypedContext->HasOrigin())
	{
		Distance = FVector::Dist(TypedContext->GetOrigin(), ImpactLocation);
	}
	else if (EffectCauser)
	{
		Distance = FVector::Dist(EffectCauser->GetActorLocation(), ImpactLocation);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Damage Calculation cannot deduce a source location for damage coming from %s; Falling back to WORLD_MAX dist!"), *GetPathNameSafe(Spec.Def));
	}

	// Apply ability source modifiers
	float PhysicalMaterialAttenuation = 1.0f;
	float DistanceAttenuation = 1.0f;
	if (const ICoreAbilitySourceInterface* AbilitySource = TypedContext->GetAbilitySource())
	{
		if (const UPhysicalMaterial* PhysMat = TypedContext->GetPhysicalMaterial())
		{
			PhysicalMaterialAttenuation = AbilitySource->GetPhysicalMaterialAttenuation(PhysMat, SourceTags, TargetTags);
		}

		DistanceAttenuation = AbilitySource->GetDistanceAttenuation(Distance, SourceTags, TargetTags);
	}
	DistanceAttenuation = FMath::Max(DistanceAttenuation, 0.0f);

	// Clamping is done when damage is converted to -health
	const float DamageDone = FMath::Max(BaseDamage * DistanceAttenuation * PhysicalMaterialAttenuation * DamageInteractionAllowedMultiplier, 0.0f);

	if (DamageDone > 0.0f)
	{
		// Apply a damage modifier, this gets turned into - health on the target
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UCoreHealthSet::GetDamageAttribute(), EGameplayModOp::Additive, DamageDone));
	}
#endif // #if WITH_SERVER_CODE
}
