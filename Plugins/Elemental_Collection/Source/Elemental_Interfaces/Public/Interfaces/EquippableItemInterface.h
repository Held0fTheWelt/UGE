// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EquippableItemInterface.generated.h"

/**
 * Interface for inventory items that can be equipped.
 * Returns the equipment definition class (e.g. UEquipmentDefinition) as UObject subclass.
 */
UINTERFACE(MinimalAPI, BlueprintType)
class UEquippableItemInterface : public UInterface
{
	GENERATED_BODY()
};

class ELEMENTAL_INTERFACES_API IEquippableItemInterface
{
	GENERATED_BODY()

public:
	/** Returns the equipment definition class to use when equipping this item. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Equipment")
	TSubclassOf<UObject> GetEquipmentDefinitionClass() const;
	virtual TSubclassOf<UObject> GetEquipmentDefinitionClass_Implementation() const { return nullptr; }
};
