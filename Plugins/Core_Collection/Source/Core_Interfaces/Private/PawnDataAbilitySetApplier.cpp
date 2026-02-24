// Copyright Epic Games, Inc. All Rights Reserved.

#include "PawnDataAbilitySetApplier.h"
#include "AbilitySystemComponent.h"

namespace PawnDataAbilitySetApplier
{
	static FApplier GApplier;

	void SetApplier(FApplier Applier)
	{
		GApplier = MoveTemp(Applier);
	}

	void Invoke(UAbilitySystemComponent* ASC, const TArray<UObject*>& AbilitySets, UObject* SourceObject)
	{
		if (GApplier && ASC)
		{
			GApplier(ASC, AbilitySets, SourceObject);
		}
	}
}
