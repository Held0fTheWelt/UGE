// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "InventoryItemDefinition.h"
#include "InventoryFragment_PickupIcon.generated.h"

/**
 * UInventoryFragment_PickupIcon
 *
 *	Fragment that defines the pickup icon for an item.
 *	Adapted from ULyraInventoryFragment_PickupIcon.
 */
UCLASS()
class GAMEFEATURE_INVENTORYSYSTEMRUNTIME_API UInventoryFragment_PickupIcon : public UInventoryItemFragment
{
	GENERATED_BODY()

public:
	// Icon to display when this item is picked up
	UPROPERTY(EditDefaultsOnly, Category = "Icon")
	TSoftObjectPtr<UTexture2D> PickupIcon;
};
