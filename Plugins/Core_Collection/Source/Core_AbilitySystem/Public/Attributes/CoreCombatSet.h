// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#pragma once

#include "AbilitySystemComponent.h"
#include "Attributes/CoreAttributeSet.h"

#include "CoreCombatSet.generated.h"

class UObject;
struct FFrame;


/**
 * UCoreCombatSet
 *
 *  Class that defines attributes that are necessary for applying damage or healing.
 *	Attribute examples include: damage, healing, attack power, and shield penetrations.
 *	Adapted from ULyraCombatSet.
 */
UCLASS(BlueprintType)
class CORE_ABILITYSYSTEM_API UCoreCombatSet : public UCoreAttributeSet
{
	GENERATED_BODY()

public:

	UCoreCombatSet();

	ATTRIBUTE_ACCESSORS(UCoreCombatSet, BaseDamage);
	ATTRIBUTE_ACCESSORS(UCoreCombatSet, BaseHeal);

protected:

	UFUNCTION()
	void OnRep_BaseDamage(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_BaseHeal(const FGameplayAttributeData& OldValue);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

	// The base amount of damage to apply in the damage execution.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BaseDamage, Category = "Core|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseDamage;

	// The base amount of healing to apply in the heal execution.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BaseHeal, Category = "Core|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseHeal;
};
