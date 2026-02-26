// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "InventoryItemDefinition.h"
#include "Interfaces/EquipmentDefinitionInterface.h"
#include "InventoryFragment_EquippableItem.generated.h"

class UObject;

/**
 * UInventoryFragment_EquippableItem
 *
 *	Fragment that marks an item as equippable.
 *	EquipmentDefinitionClass should implement IEquipmentDefinitionInterface (e.g. from Core_Collection) when available.
 */
UCLASS()
class GAMEFEATURE_INVENTORYSYSTEMRUNTIME_API UInventoryFragment_EquippableItem : public UInventoryItemFragment
{
	GENERATED_BODY()

public:
	// Slot tag for this equippable item
	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	FGameplayTag SlotTag;

	// Equipment definition class (implement IEquipmentDefinitionInterface for slot/behavior; to be provided by Core/Equipment plugin)
	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	TSubclassOf<UObject> EquipmentDefinitionClass;
};
