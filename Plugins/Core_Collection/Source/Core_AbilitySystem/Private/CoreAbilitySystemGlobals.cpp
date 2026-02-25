// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#include "CoreAbilitySystemGlobals.h"

#include "CoreGameplayEffectContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreAbilitySystemGlobals)

struct FGameplayEffectContext;

UCoreAbilitySystemGlobals::UCoreAbilitySystemGlobals(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FGameplayEffectContext* UCoreAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FCoreGameplayEffectContext();
}
