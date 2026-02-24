// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#pragma once

#include "AbilitySystemComponent.h"
#include "Attributes/CoreAttributeSet.h"
#include "NativeGameplayTags.h"

#include "CoreHealthSet.generated.h"

class UObject;
struct FFrame;

CORE_ABILITYSYSTEM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Damage);
CORE_ABILITYSYSTEM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_DamageImmunity);
CORE_ABILITYSYSTEM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_DamageSelfDestruct);
CORE_ABILITYSYSTEM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_FellOutOfWorld);
CORE_ABILITYSYSTEM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Core_Damage_Message);

struct FGameplayEffectModCallbackData;


/**
 * UCoreHealthSet
 *
 *	Class that defines attributes that are necessary for taking damage.
 *	Attribute examples include: health, shields, and resistances.
 *	Adapted from ULyraHealthSet.
 */
UCLASS(BlueprintType)
class CORE_ABILITYSYSTEM_API UCoreHealthSet : public UCoreAttributeSet
{
	GENERATED_BODY()

public:

	UCoreHealthSet();

	ATTRIBUTE_ACCESSORS(UCoreHealthSet, Health);
	ATTRIBUTE_ACCESSORS(UCoreHealthSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UCoreHealthSet, Healing);
	ATTRIBUTE_ACCESSORS(UCoreHealthSet, Damage);

	// Delegate when health changes due to damage/healing, some information may be missing on the client
	mutable FCoreAttributeEvent OnHealthChanged;

	// Delegate when max health changes
	mutable FCoreAttributeEvent OnMaxHealthChanged;

	// Delegate to broadcast when the health attribute reaches zero
	mutable FCoreAttributeEvent OnOutOfHealth;

protected:

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

	// The current health attribute.  The health will be capped by the max health attribute.  Health is hidden from modifiers so only executions can modify it.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Core|Health", Meta = (HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	// The current max health attribute.  Max health is an attribute since gameplay effects can modify it.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Core|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	// Used to track when the health reaches 0.
	bool bOutOfHealth;

	// Store the health before any changes 
	float MaxHealthBeforeAttributeChange;
	float HealthBeforeAttributeChange;

	// -------------------------------------------------------------------
	//	Meta Attribute (please keep attributes that aren't 'stateful' below 
	// -------------------------------------------------------------------

	// Incoming healing. This is mapped directly to +Health
	UPROPERTY(BlueprintReadOnly, Category="Core|Health", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Healing;

	// Incoming damage. This is mapped directly to -Health
	UPROPERTY(BlueprintReadOnly, Category="Core|Health", Meta=(HideFromModifiers, AllowPrivateAccess=true))
	FGameplayAttributeData Damage;
};
