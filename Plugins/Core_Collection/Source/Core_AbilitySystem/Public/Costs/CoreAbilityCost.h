// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayAbilitySpec.h"
#include "CoreAbilityCost.generated.h"

class UGameplayAbility;
struct FGameplayAbilityActorInfo;
struct FGameplayAbilityActivationInfo;
struct FGameplayTagContainer;

/**
 * UCoreAbilityCost
 *
 *	Base class for costs that a Core ability can have.
 *	Adapted from ULyraAbilityCost.
 */
UCLASS(Abstract, EditInlineNew)
class CORE_ABILITYSYSTEM_API UCoreAbilityCost : public UObject
{
	GENERATED_BODY()

public:
	UCoreAbilityCost();

	/**
	 * Checks if we can afford this cost.
	 *
	 * @param Ability			The ability that is wanting to activate
	 * @param Handle			The ability spec handle
	 * @param ActorInfo			The actor info
	 * @param OptionalRelevantTags	If not null, populated with tags that explain why this cost can/cannot be paid
	 *
	 * @return true if we can pay for the ability, false otherwise.
	 */
	virtual bool CheckCost(const UGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags = nullptr) const
	{
		return true;
	}

	/**
	 * Applies the ability cost.
	 *
	 * @param Ability			The ability to apply the cost to
	 * @param Handle			The ability spec handle
	 * @param ActorInfo			The actor info
	 * @param ActivationInfo	The activation info
	 */
	virtual void ApplyCost(const UGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo& ActivationInfo)
	{
	}

	/**
	 * Returns true if this cost should only be applied when the ability hits a target.
	 * This is used for things like ammo - you don't want to consume ammo unless you actually hit something.
	 */
	virtual bool ShouldOnlyApplyCostOnHit() const
	{
		return bOnlyApplyCostOnHit;
	}

protected:
	/** If true, this cost should only be applied if this ability hits successfully */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Costs)
	bool bOnlyApplyCostOnHit = false;
};
