// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryItemDefinitionInterface.generated.h"

class UObject;

/**
 * Interface for item definitions that carry fragments (e.g. inventory item definition).
 * Other GameFeatures can query fragments by class via this interface without depending on the inventory module.
 */
UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UInventoryItemDefinitionInterface : public UInterface
{
	GENERATED_BODY()
};

class ELEMENTAL_INTERFACES_API IInventoryItemDefinitionInterface
{
	GENERATED_BODY()

public:
	/** Returns the first fragment of the given class, or nullptr. Fragment classes implement IInventoryItemFragmentInterface. */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual const UObject* FindFragmentByClass(TSubclassOf<UObject> FragmentClass) const { return nullptr; }
};
