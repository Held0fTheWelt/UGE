// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#include "Attributes/CoreAttributeSet.h"

#include "Components/CoreAbilitySystemComponent.h"
#include "Engine/World.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreAttributeSet)

UCoreAttributeSet::UCoreAttributeSet()
{
}

UWorld* UCoreAttributeSet::GetWorld() const
{
	const UObject* Outer = GetOuter();
	check(Outer);

	return Outer->GetWorld();
}

UCoreAbilitySystemComponent* UCoreAttributeSet::GetCoreAbilitySystemComponent() const
{
	return Cast<UCoreAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}
