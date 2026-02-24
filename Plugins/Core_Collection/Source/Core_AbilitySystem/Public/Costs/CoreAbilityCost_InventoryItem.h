// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#pragma once

#include "Costs/CoreAbilityCost.h"
#include "ScalableFloat.h"
#include "Templates/SubclassOf.h"

#include "CoreAbilityCost_InventoryItem.generated.h"

struct FGameplayAbilityActivationInfo;
struct FGameplayAbilitySpecHandle;

class UCoreGameplayAbility;
class UObject;
struct FGameplayAbilityActorInfo;
struct FGameplayTagContainer;

class UActorComponent;
class UCoreInventoryItemDefinition;

/**
 * Represents a cost that requires expending a quantity of an inventory item.
 * Uses reflection to call GameFeature_InventorySystemRuntime's CoreInventoryManagerComponent when present.
 */
UCLASS(meta=(DisplayName="Inventory Item"))
class CORE_ABILITYSYSTEM_API UCoreAbilityCost_InventoryItem : public UCoreAbilityCost
{
	GENERATED_BODY()

public:
	UCoreAbilityCost_InventoryItem();

	//~UCoreAbilityCost interface
	virtual bool CheckCost(const UGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ApplyCost(const UGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo& ActivationInfo) override;
	//~End of UCoreAbilityCost interface

protected:
	/** How much of the item to spend (keyed on ability level) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=AbilityCost)
	FScalableFloat Quantity;

	/** Which item to consume */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=AbilityCost, meta=(MustImplement="Interface"))
	TSubclassOf<UObject> ItemDefinition;

	/** Returns the inventory manager component (CoreInventoryManagerComponent) from the ability's controller when GameFeature_InventorySystemRuntime is loaded; otherwise nullptr. */
	virtual UActorComponent* GetInventoryManagerComponent(const UGameplayAbility* Ability) const;
};
