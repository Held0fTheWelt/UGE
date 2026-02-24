// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#pragma once

#include "Costs/CoreAbilityCost.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"

#include "CoreAbilityCost_ItemTagStack.generated.h"

struct FGameplayAbilityActivationInfo;
struct FGameplayAbilitySpecHandle;

class UCoreGameplayAbility;
class UObject;
struct FGameplayAbilityActorInfo;

/**
 * Represents a cost that requires expending a quantity of a tag stack
 * on the associated item instance.
 * 
 * Note: This requires the ability to have access to an inventory item instance.
 * For equipment-based abilities, the ability should implement a method to get the associated item.
 */
UCLASS(meta=(DisplayName="Item Tag Stack"))
class CORE_ABILITYSYSTEM_API UCoreAbilityCost_ItemTagStack : public UCoreAbilityCost
{
	GENERATED_BODY()

public:
	UCoreAbilityCost_ItemTagStack();

	//~UCoreAbilityCost interface
	virtual bool CheckCost(const UGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ApplyCost(const UGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo& ActivationInfo) override;
	//~End of UCoreAbilityCost interface

protected:
	/** How much of the tag to spend (keyed on ability level) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Costs)
	FScalableFloat Quantity;

	/** Which tag to spend some of */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Costs)
	FGameplayTag Tag;

	/** Which tag to send back as a response if this cost cannot be applied */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Costs)
	FGameplayTag FailureTag;

	/** Helper function to get the associated item instance from the ability */
	virtual UObject* GetAssociatedItem(const UGameplayAbility* Ability) const;
};
