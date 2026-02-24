// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "InventoryItemStatsInterface.generated.h"

/**
 * Interface for item definitions that expose stat values by tag.
 * Used e.g. by weapon spawners to read default stats from an inventory item definition class.
 */
UINTERFACE(MinimalAPI, BlueprintType)
class UInventoryItemStatsInterface : public UInterface
{
	GENERATED_BODY()
};

class ELEMENTAL_INTERFACES_API IInventoryItemStatsInterface
{
	GENERATED_BODY()

public:
	/** Returns the value of the item stat with the given tag, or 0 if not found. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
	int32 GetItemStatByTag(FGameplayTag StatTag) const;
	virtual int32 GetItemStatByTag_Implementation(FGameplayTag StatTag) const { return 0; }
};
