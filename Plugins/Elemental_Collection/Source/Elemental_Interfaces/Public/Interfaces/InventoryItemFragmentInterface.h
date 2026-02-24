// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryItemFragmentInterface.generated.h"

/**
 * Interface for item-definition fragments (e.g. stats, reticle config).
 * GameFeatures implement this so fragments can be attached to item definitions without
 * GameFeatures depending on each other. The inventory system calls OnInstanceCreated when an item instance is created.
 */
UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UInventoryItemFragmentInterface : public UInterface
{
	GENERATED_BODY()
};

class ELEMENTAL_INTERFACES_API IInventoryItemFragmentInterface
{
	GENERATED_BODY()

public:
	/** Called when an item instance is created from a definition that contains this fragment. ItemInstance implements IInventoryItemInstanceInterface. */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual void OnInstanceCreated(UObject* ItemInstance) const {}
};
