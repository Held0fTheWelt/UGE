// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#pragma once

#include "Costs/CoreAbilityCost.h"
#include "CoreAbilityCost_PlayerTagStack.generated.h"

class UGameplayAbility;
struct FGameplayAbilityActorInfo;
struct FGameplayAbilityActivationInfo;
struct FGameplayAbilitySpecHandle;
struct FGameplayTagContainer;

/**
 * UCoreAbilityCost_PlayerTagStack
 *
 *	Cost that consumes a specified number of stacks from a gameplay tag on the player state.
 *	Adapted from ULyraAbilityCost_PlayerTagStack.
 */
UCLASS(meta = (DisplayName = "Player Tag Stack Cost"))
class CORE_ABILITYSYSTEM_API UCoreAbilityCost_PlayerTagStack : public UCoreAbilityCost
{
	GENERATED_BODY()

public:
	UCoreAbilityCost_PlayerTagStack();

	//~UCoreAbilityCost interface
	virtual bool CheckCost(const UGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ApplyCost(const UGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo& ActivationInfo) override;
	//~End of UCoreAbilityCost interface

protected:
	/** Gameplay tag that identifies the tag stack we want to consume from */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Costs)
	FGameplayTag QuantityTag;

	/** How much of the tag stack to consume (cost) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Costs, meta = (ClampMin = "1"))
	int32 Quantity = 1;

	/** If true, the cost will be applied even if the ability fails to activate */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Costs)
	bool bFailIfTagIsMissing = true;
};
