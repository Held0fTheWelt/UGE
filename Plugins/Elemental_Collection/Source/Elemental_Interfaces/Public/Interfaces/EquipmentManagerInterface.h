// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EquipmentManagerInterface.generated.h"

class UActorComponent;

/**
 * Interface for pawns that have an equipment manager component.
 * Allows other systems (e.g. QuickBar) to resolve the manager via interface instead of concrete class.
 */
UINTERFACE(MinimalAPI, BlueprintType)
class UEquipmentManagerInterface : public UInterface
{
	GENERATED_BODY()
};

class ELEMENTAL_INTERFACES_API IEquipmentManagerInterface
{
	GENERATED_BODY()

public:
	/** Returns the equipment manager component on this actor, or nullptr. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Equipment")
	UActorComponent* GetEquipmentManagerComponent() const;
	virtual UActorComponent* GetEquipmentManagerComponent_Implementation() const { return nullptr; }
};
