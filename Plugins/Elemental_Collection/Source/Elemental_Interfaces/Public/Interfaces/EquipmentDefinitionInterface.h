// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "EquipmentDefinitionInterface.generated.h"

/**
 * Interface for equipment definitions (e.g. used by equippable inventory fragments).
 * To be implemented later by Core_Collection or the Equipment plugin; Inventory System only stores TSubclassOf<UObject> and queries via this interface.
 */
UINTERFACE(MinimalAPI, BlueprintType)
class UEquipmentDefinitionInterface : public UInterface
{
	GENERATED_BODY()
};

class ELEMENTAL_INTERFACES_API IEquipmentDefinitionInterface
{
	GENERATED_BODY()

public:
	/** Returns the slot tag for this equipment definition. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Equipment")
	FGameplayTag GetSlotTag() const;
	virtual FGameplayTag GetSlotTag_Implementation() const { return FGameplayTag(); }
};
