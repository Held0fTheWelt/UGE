// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "Fragments/InventoryFragment_SetStats.h"
#include "InventoryItemInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InventoryFragment_SetStats)

void UInventoryFragment_SetStats::OnInstanceCreatedForSubclass(UInventoryItemInstance* Instance) const
{
	for (const auto& Entry : InitialItemStats)
	{
		Instance->AddStatTagStack(Entry.Key, Entry.Value);
	}
}

int32 UInventoryFragment_SetStats::GetItemStatByTag_Implementation(FGameplayTag StatTag) const
{
	if (const int32* StatPtr = InitialItemStats.Find(StatTag))
	{
		return *StatPtr;
	}

	return 0;
}