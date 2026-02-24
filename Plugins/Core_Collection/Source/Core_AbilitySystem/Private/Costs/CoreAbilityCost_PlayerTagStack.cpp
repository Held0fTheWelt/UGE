// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#include "Costs/CoreAbilityCost_PlayerTagStack.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerState.h"
#include "GameplayTagContainer.h"
#include "States/PlayerState_Elemental.h"
#include "System/GameplayTagStack.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreAbilityCost_PlayerTagStack)

UCoreAbilityCost_PlayerTagStack::UCoreAbilityCost_PlayerTagStack()
{
	Quantity = 1;
	bFailIfTagIsMissing = true;
}

bool UCoreAbilityCost_PlayerTagStack::CheckCost(const UGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (ActorInfo == nullptr)
	{
		return false;
	}

	// Find the player state (from avatar pawn when available)
	APlayerState* PS = nullptr;
	if (ActorInfo->AvatarActor.IsValid())
	{
		if (APawn* Pawn = Cast<APawn>(ActorInfo->AvatarActor.Get()))
		{
			PS = Pawn->GetPlayerState();
		}
	}
	if (PS == nullptr)
	{
		return false;
	}

	const APlayerState_Elemental* ElementalPS = Cast<APlayerState_Elemental>(PS);
	if (ElementalPS == nullptr)
	{
		return false;
	}

	// Check if we have enough stacks
	const int32 CurrentStacks = ElementalPS->GetStatTagStackCount(QuantityTag);
	const bool bCanAfford = (CurrentStacks >= Quantity);

	if (!bCanAfford && OptionalRelevantTags && bFailIfTagIsMissing)
	{
		OptionalRelevantTags->AddTag(QuantityTag);
	}

	return bCanAfford;
}

void UCoreAbilityCost_PlayerTagStack::ApplyCost(const UGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo& ActivationInfo)
{
	if (ActorInfo->IsNetAuthority())
	{
		APlayerState* PS = nullptr;
		if (ActorInfo->AvatarActor.IsValid())
		{
			if (APawn* Pawn = Cast<APawn>(ActorInfo->AvatarActor.Get()))
			{
				PS = Pawn->GetPlayerState();
			}
		}
		if (PS == nullptr)
		{
			return;
		}

		APlayerState_Elemental* ElementalPS = Cast<APlayerState_Elemental>(PS);
		if (ElementalPS == nullptr)
		{
			return;
		}

		ElementalPS->RemoveStatTagStack(QuantityTag, Quantity);
	}
}
