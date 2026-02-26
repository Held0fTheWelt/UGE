// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "InventoryItemDefinition.h"
#include "InventoryFragment_QuickBarIcon.generated.h"

/**
 * UInventoryFragment_QuickBarIcon
 *
 *	Fragment that defines the quick bar icon for an item.
 *	Adapted from ULyraInventoryFragment_QuickBarIcon.
 */
UCLASS()
class GAMEFEATURE_INVENTORYSYSTEMRUNTIME_API UInventoryFragment_QuickBarIcon : public UInventoryItemFragment
{
	GENERATED_BODY()

public:
	// Icon to display in the quick bar
	UPROPERTY(EditDefaultsOnly, Category = "Icon")
	TSoftObjectPtr<UTexture2D> QuickBarIcon;
};
