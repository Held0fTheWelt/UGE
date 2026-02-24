// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "Interfaces/AbilitySetInterface.h"
#include "AbilitySystemComponent.h"

void FAbilitySetGrantedHandles::TakeFromAbilitySystem(UAbilitySystemComponent* ASC)
{
	if (!ASC) return;

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			ASC->ClearAbility(Handle);
		}
	}
	AbilitySpecHandles.Empty();

	for (FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			ASC->RemoveActiveGameplayEffect(Handle);
		}
	}
	GameplayEffectHandles.Empty();
}
