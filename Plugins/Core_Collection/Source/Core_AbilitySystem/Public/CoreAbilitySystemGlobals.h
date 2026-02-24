// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#pragma once

#include "AbilitySystemGlobals.h"

#include "CoreAbilitySystemGlobals.generated.h"

class UObject;
struct FGameplayEffectContext;

/**
 * UCoreAbilitySystemGlobals
 *
 *	Custom Ability System Globals for Core.
 *	Adapted from LyraAbilitySystemGlobals.
 */
UCLASS(Config=Game)
class CORE_ABILITYSYSTEM_API UCoreAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_UCLASS_BODY()

	//~UAbilitySystemGlobals interface
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	//~End of UAbilitySystemGlobals interface
};
