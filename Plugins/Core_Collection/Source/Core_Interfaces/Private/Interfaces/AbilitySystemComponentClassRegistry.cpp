// Copyright Epic Games, Inc. All Rights Reserved.

#include "Interfaces/AbilitySystemComponentClassRegistry.h"

namespace CoreAbilitySystemComponentClassRegistry
{
	static UClass* GAbilitySystemComponentClass = nullptr;

	UClass* GetAbilitySystemComponentClass()
	{
		return GAbilitySystemComponentClass;
	}

	void SetAbilitySystemComponentClass(UClass* InClass)
	{
		GAbilitySystemComponentClass = InClass;
	}
}
