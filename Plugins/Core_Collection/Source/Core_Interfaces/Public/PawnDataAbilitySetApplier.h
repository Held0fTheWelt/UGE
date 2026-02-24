// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class UAbilitySystemComponent;
class UObject;

/**
 * Registry for the pawn data ability set applier so Core_System's PawnData_Core can grant ability sets
 * without depending on Core_AbilitySystem. Core_AbilitySystem registers its applier at startup.
 */
namespace PawnDataAbilitySetApplier
{
	using FApplier = TFunction<void(UAbilitySystemComponent* ASC, const TArray<UObject*>& AbilitySets, UObject* SourceObject)>;

	CORE_INTERFACES_API void SetApplier(FApplier Applier);
	CORE_INTERFACES_API void Invoke(UAbilitySystemComponent* ASC, const TArray<UObject*>& AbilitySets, UObject* SourceObject);
}
