// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryItemInstanceInterface.generated.h"

class UObject;

/**
 * Interface for an inventory item instance (runtime object representing a stacked item).
 * Implemented by the Inventory System; other modules can work with item instances via this interface.
 */
UINTERFACE(MinimalAPI, BlueprintType)
class UInventoryItemInstanceInterface : public UInterface
{
	GENERATED_BODY()
};

class ELEMENTAL_INTERFACES_API IInventoryItemInstanceInterface
{
	GENERATED_BODY()

public:
	/** Returns the item definition class for this instance. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
	TSubclassOf<UObject> GetItemDefinitionClass() const;
	virtual TSubclassOf<UObject> GetItemDefinitionClass_Implementation() const { return nullptr; }

	/** Returns the first fragment of the given class from this item's definition, or nullptr. Use this so GameFeatures can query fragments without depending on each other. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
	const UObject* FindFragmentByClass(TSubclassOf<UObject> FragmentClass) const;
	virtual const UObject* FindFragmentByClass_Implementation(TSubclassOf<UObject> FragmentClass) const { return nullptr; }
};
