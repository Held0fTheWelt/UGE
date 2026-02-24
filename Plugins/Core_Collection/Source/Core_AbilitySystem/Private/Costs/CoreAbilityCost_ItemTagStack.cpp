// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#include "Costs/CoreAbilityCost_ItemTagStack.h"

#include "Abilities/CoreGameplayAbility.h"
#include "NativeGameplayTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreAbilityCost_ItemTagStack)

UE_DEFINE_GAMEPLAY_TAG(TAG_ABILITY_FAIL_COST, "Ability.ActivateFail.Cost");

UCoreAbilityCost_ItemTagStack::UCoreAbilityCost_ItemTagStack()
{
	Quantity.SetValue(1.0f);
	FailureTag = TAG_ABILITY_FAIL_COST;
}

UObject* UCoreAbilityCost_ItemTagStack::GetAssociatedItem(const UGameplayAbility* Ability) const
{
	// Try to get the item from the ability's source actor
	// This is a generic implementation - specific abilities can override this behavior
	// by implementing a method to get the associated item
	
	// For now, return nullptr - this should be implemented by abilities that need item-based costs
	// TODO: Add interface or method to get associated item from ability
	// The actual implementation should use reflection or an interface to get the item instance
	return nullptr;
}

bool UCoreAbilityCost_ItemTagStack::CheckCost(const UGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (UObject* ItemInstance = GetAssociatedItem(Ability))
	{
		// Use reflection to call GetStatTagStackCount to avoid direct dependency
		if (UFunction* GetStatTagStackCountFunc = ItemInstance->FindFunction(TEXT("GetStatTagStackCount")))
		{
			struct FGetStatTagStackCountParams
			{
				FGameplayTag Tag;
				int32 ReturnValue;
			};
			FGetStatTagStackCountParams Params;
			Params.Tag = Tag;
			Params.ReturnValue = 0;
			
			ItemInstance->ProcessEvent(GetStatTagStackCountFunc, &Params);
			
			const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);
			const float NumStacksReal = Quantity.GetValueAtLevel(AbilityLevel);
			const int32 NumStacks = FMath::TruncToInt(NumStacksReal);
			const bool bCanApplyCost = Params.ReturnValue >= NumStacks;

			// Inform other abilities why this cost cannot be applied
			if (!bCanApplyCost && OptionalRelevantTags && FailureTag.IsValid())
			{
				OptionalRelevantTags->AddTag(FailureTag);				
			}
			return bCanApplyCost;
		}
	}
	return false;
}

void UCoreAbilityCost_ItemTagStack::ApplyCost(const UGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo& ActivationInfo)
{
	if (ActorInfo->IsNetAuthority())
	{
		if (UObject* ItemInstance = GetAssociatedItem(Ability))
		{
			// Use reflection to call RemoveStatTagStack to avoid direct dependency
			if (UFunction* RemoveStatTagStackFunc = ItemInstance->FindFunction(TEXT("RemoveStatTagStack")))
			{
				struct FRemoveStatTagStackParams
				{
					FGameplayTag Tag;
					int32 StackCount;
				};
				FRemoveStatTagStackParams Params;
				Params.Tag = Tag;
				
				const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);
				const float NumStacksReal = Quantity.GetValueAtLevel(AbilityLevel);
				Params.StackCount = FMath::TruncToInt(NumStacksReal);

				ItemInstance->ProcessEvent(RemoveStatTagStackFunc, &Params);
			}
		}
	}
}
