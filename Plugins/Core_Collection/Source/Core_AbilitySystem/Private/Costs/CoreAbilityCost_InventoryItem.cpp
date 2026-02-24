// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#include "Costs/CoreAbilityCost_InventoryItem.h"

#include "Abilities/CoreGameplayAbility.h"
#include "GameplayAbilitySpec.h"
#include "GameplayAbilitySpecHandle.h"

#include "GameFramework/Actor.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreAbilityCost_InventoryItem)

UCoreAbilityCost_InventoryItem::UCoreAbilityCost_InventoryItem()
{
	Quantity.SetValue(1.0f);
}

UActorComponent* UCoreAbilityCost_InventoryItem::GetInventoryManagerComponent(const UGameplayAbility* Ability) const
{
	if (const UCoreGameplayAbility* CoreAbility = Cast<const UCoreGameplayAbility>(Ability))
	{
		if (AController* PC = CoreAbility->GetControllerFromActorInfo())
		{
			UClass* InventoryManagerClass = UClass::TryFindTypeSlow<UClass>(TEXT("/Script/GameFeature_InventorySystemRuntime.CoreInventoryManagerComponent"));
			if (InventoryManagerClass)
			{
				return PC->FindComponentByClass(InventoryManagerClass);
			}
		}
	}
	return nullptr;
}

bool UCoreAbilityCost_InventoryItem::CheckCost(const UGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (UActorComponent* InventoryComponent = GetInventoryManagerComponent(Ability))
	{
		if (ItemDefinition)
		{
			const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);

			const float NumItemsToConsumeReal = Quantity.GetValueAtLevel(AbilityLevel);
			const int32 NumItemsToConsume = FMath::TruncToInt(NumItemsToConsumeReal);

			if (UFunction* GetTotalItemCountFunc = InventoryComponent->FindFunction(TEXT("GetTotalItemCountByDefinition")))
			{
				struct FGetTotalItemCountParams
				{
					TSubclassOf<UObject> ItemDef;
					int32 ReturnValue;
				};
				FGetTotalItemCountParams Params;
				Params.ItemDef = ItemDefinition;
				Params.ReturnValue = 0;
				
				InventoryComponent->ProcessEvent(GetTotalItemCountFunc, &Params);
				return Params.ReturnValue >= NumItemsToConsume;
			}
		}
	}
	return false;
}

void UCoreAbilityCost_InventoryItem::ApplyCost(const UGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo& ActivationInfo)
{
	if (ActorInfo->IsNetAuthority())
	{
		if (UActorComponent* InventoryComponent = GetInventoryManagerComponent(Ability))
		{
			if (ItemDefinition)
			{
				const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);

				const float NumItemsToConsumeReal = Quantity.GetValueAtLevel(AbilityLevel);
				const int32 NumItemsToConsume = FMath::TruncToInt(NumItemsToConsumeReal);

				if (UFunction* ConsumeItemsFunc = InventoryComponent->FindFunction(TEXT("ConsumeItemsByDefinition")))
				{
					struct FConsumeItemsParams
					{
						TSubclassOf<UObject> ItemDef;
						int32 NumToConsume;
						bool ReturnValue;
					};
					FConsumeItemsParams Params;
					Params.ItemDef = ItemDefinition;
					Params.NumToConsume = NumItemsToConsume;
					Params.ReturnValue = false;
					
					InventoryComponent->ProcessEvent(ConsumeItemsFunc, &Params);
				}
			}
		}
	}
}
