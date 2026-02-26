// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "InventoryItemDefinition.h"
#include "Interfaces/InventoryItemStatsInterface.h"
#include "InventoryFragment_SetStats.generated.h"

class UInventoryItemInstance;

/**
 * UInventoryFragment_SetStats
 *
 *	Fragment that sets stats when an item is created.
 *	Implements IInventoryItemStatsInterface so callers can query stats via interface.
 */
UCLASS()
class GAMEFEATURE_INVENTORYSYSTEMRUNTIME_API UInventoryFragment_SetStats : public UInventoryItemFragment, public IInventoryItemStatsInterface
{
	GENERATED_BODY()

public:
	virtual void OnInstanceCreatedForSubclass(UInventoryItemInstance* Instance) const override;

	//~IInventoryItemStatsInterface
	virtual int32 GetItemStatByTag_Implementation(FGameplayTag StatTag) const override;

protected:
	// Stats to set when the item is created
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	TMap<FGameplayTag, int32> InitialItemStats;
};
