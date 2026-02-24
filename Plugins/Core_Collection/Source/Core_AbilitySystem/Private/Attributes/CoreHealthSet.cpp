// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#include "Attributes/CoreHealthSet.h"

#include "Attributes/CoreAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "Components/CoreAbilitySystemComponent.h"
#include "Engine/World.h"
#include "GameplayEffectExtension.h"
#include "Structs/VerbMessageTypes.h"
#include "GameFramework/GameplayMessageSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreHealthSet)

UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Damage, "Gameplay.Damage");
UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_DamageImmunity, "Gameplay.DamageImmunity");
UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_DamageSelfDestruct, "Gameplay.Damage.SelfDestruct");
UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_FellOutOfWorld, "Gameplay.Damage.FellOutOfWorld");
UE_DEFINE_GAMEPLAY_TAG(TAG_Core_Damage_Message, "Core.Damage.Message");

UCoreHealthSet::UCoreHealthSet()
	: Health(100.0f)
	, MaxHealth(100.0f)
{
	bOutOfHealth = false;
	MaxHealthBeforeAttributeChange = 0.0f;
	HealthBeforeAttributeChange = 0.0f;
}

void UCoreHealthSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCoreHealthSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCoreHealthSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UCoreHealthSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCoreHealthSet, Health, OldValue);

	// Call the change callback, but without an instigator
	// This could be changed to an explicit RPC in the future
	// These events on the client should not be changing attributes

	const float CurrentHealth = GetHealth();
	const float EstimatedMagnitude = CurrentHealth - OldValue.GetCurrentValue();
	
	OnHealthChanged.Broadcast(nullptr, nullptr, nullptr, EstimatedMagnitude, OldValue.GetCurrentValue(), CurrentHealth);

	if (!bOutOfHealth && CurrentHealth <= 0.0f)
	{
		OnOutOfHealth.Broadcast(nullptr, nullptr, nullptr, EstimatedMagnitude, OldValue.GetCurrentValue(), CurrentHealth);
	}

	bOutOfHealth = (CurrentHealth <= 0.0f);
}

void UCoreHealthSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCoreHealthSet, MaxHealth, OldValue);

	// Call the change callback, but without an instigator
	// This could be changed to an explicit RPC in the future
	OnMaxHealthChanged.Broadcast(nullptr, nullptr, nullptr, GetMaxHealth() - OldValue.GetCurrentValue(), OldValue.GetCurrentValue(), GetMaxHealth());
}

bool UCoreHealthSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData &Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	// Handle modifying incoming normal damage
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		if (Data.EvaluatedData.Magnitude > 0.0f)
		{
			const bool bIsDamageFromSelfDestruct = Data.EffectSpec.GetDynamicAssetTags().HasTagExact(TAG_Gameplay_DamageSelfDestruct);

			if (Data.Target.HasMatchingGameplayTag(TAG_Gameplay_DamageImmunity) && !bIsDamageFromSelfDestruct)
			{
				// Do not take away any health.
				Data.EvaluatedData.Magnitude = 0.0f;
				return false;
			}

#if !UE_BUILD_SHIPPING
			// Check GodMode cheat, unlimited health is checked below
			// TODO: Add Core gameplay tags for cheats if needed
			// if (Data.Target.HasMatchingGameplayTag(CoreGameplayTags::Cheat_GodMode) && !bIsDamageFromSelfDestruct)
			// {
			// 	// Do not take away any health.
			// 	Data.EvaluatedData.Magnitude = 0.0f;
			// 	return false;
			// }
#endif // #if !UE_BUILD_SHIPPING
		}
	}

	return true;
}

void UCoreHealthSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	float MinimumHealth = 0.0f;

	// Clamp health to max health
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), MinimumHealth, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Convert incoming damage to health change
		const float LocalDamageDone = GetDamage();
		const float OldHealth = GetHealth();
		float NewHealth = FMath::Clamp(OldHealth - LocalDamageDone, MinimumHealth, GetMaxHealth());

		SetHealth(NewHealth);
		SetDamage(0.0f);

		if (LocalDamageDone > 0.0f)
		{
			const bool bWasAlive = OldHealth > 0.0f;
			const bool bIsAlive = NewHealth > 0.0f;

			// Send a standardized verb message when damage occurs
			FVerbMessage Message;
			Message.Verb = TAG_Core_Damage_Message;
			Message.Instigator = Data.EffectSpec.GetEffectContext().GetOriginalInstigator();
			Message.InstigatorTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
			Message.Target = GetOwningAbilitySystemComponent()->GetAvatarActor();
			Message.TargetTags = *Data.EffectSpec.CapturedTargetTags.GetAggregatedTags();
			Message.Magnitude = LocalDamageDone;

			UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
			MessageSystem.BroadcastMessage(Message.Verb, Message);

			// Send health change event
			OnHealthChanged.Broadcast(Data.EffectSpec.GetEffectContext().GetOriginalInstigator(), Data.EffectSpec.GetEffectContext().GetEffectCauser(), &Data.EffectSpec, LocalDamageDone, OldHealth, NewHealth);

			if (bWasAlive && !bIsAlive)
			{
				// This damage killed us
				OnOutOfHealth.Broadcast(Data.EffectSpec.GetEffectContext().GetOriginalInstigator(), Data.EffectSpec.GetEffectContext().GetEffectCauser(), &Data.EffectSpec, LocalDamageDone, OldHealth, NewHealth);
			}
		}
	}
	else if (Data.EvaluatedData.Attribute == GetHealingAttribute())
	{
		// Convert incoming healing to health change
		const float LocalHealingDone = GetHealing();
		const float OldHealth = GetHealth();
		float NewHealth = FMath::Clamp(OldHealth + LocalHealingDone, MinimumHealth, GetMaxHealth());

		SetHealth(NewHealth);
		SetHealing(0.0f);

		if (LocalHealingDone > 0.0f)
		{
			OnHealthChanged.Broadcast(Data.EffectSpec.GetEffectContext().GetOriginalInstigator(), Data.EffectSpec.GetEffectContext().GetEffectCauser(), &Data.EffectSpec, LocalHealingDone, OldHealth, NewHealth);
		}
	}
}

void UCoreHealthSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UCoreHealthSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UCoreHealthSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	// Make sure current health is not greater than the new max health.
	if (Attribute == GetMaxHealthAttribute())
	{
		float NewHealth = GetHealth();
		ClampAttribute(GetHealthAttribute(), NewHealth);
		if (NewHealth != GetHealth() && GetOwningAbilitySystemComponent())
		{
			GetOwningAbilitySystemComponent()->SetNumericAttributeBase(GetHealthAttribute(), NewHealth);
		}
	}
}

void UCoreHealthSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		// Do not allow health to go negative or above max health.
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		// Do not allow max health to drop below 1.
		NewValue = FMath::Max(NewValue, 1.0f);
	}
}
