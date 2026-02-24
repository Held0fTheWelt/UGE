// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#include "Attributes/CoreCombatSet.h"

#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreCombatSet)

UCoreCombatSet::UCoreCombatSet()
{
}

void UCoreCombatSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCoreCombatSet, BaseDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCoreCombatSet, BaseHeal, COND_None, REPNOTIFY_Always);
}

void UCoreCombatSet::OnRep_BaseDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCoreCombatSet, BaseDamage, OldValue);
}

void UCoreCombatSet::OnRep_BaseHeal(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCoreCombatSet, BaseHeal, OldValue);
}
