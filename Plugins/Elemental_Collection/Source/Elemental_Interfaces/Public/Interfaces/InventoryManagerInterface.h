// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryManagerInterface.generated.h"

class UObject;

/**
 * Interface for components that manage an inventory (e.g. pawn inventory).
 * Implemented by the Inventory System; other modules (e.g. Core_Collection) can query/add items via this interface.
 */
UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UInventoryManagerInterface : public UInterface
{
	GENERATED_BODY()
};

class ELEMENTAL_INTERFACES_API IInventoryManagerInterface
{
	GENERATED_BODY()

public:
	/** Returns all inventory item instances (objects implementing IInventoryItemInstanceInterface). */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual TArray<UObject*> GetInventoryItems() const { return {}; }

	/** Returns true if the given item definition class can be added. */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual bool CanAddItemDefinition(TSubclassOf<UObject> ItemDefClass, int32 StackCount) const { return false; }

	/** Adds an item by definition class; returns the new item instance or nullptr. */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual UObject* AddItemDefinition(TSubclassOf<UObject> ItemDefClass, int32 StackCount) { return nullptr; }

	/** Finds the first item stack matching the given definition class. */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual UObject* FindFirstItemStackByDefinition(TSubclassOf<UObject> ItemDefClass) const { return nullptr; }

	/** Returns total stack count for the given definition class. */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual int32 GetTotalItemCountByDefinition(TSubclassOf<UObject> ItemDefClass) const { return 0; }
};
