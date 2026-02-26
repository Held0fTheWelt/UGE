// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "Interfaces/InventoryManagerInterface.h"
#include "InventorySubsystem.generated.h"

class AActor;

/**
 * World subsystem for tag-based inventory lookup.
 * Resolves inventory components by owner actor and optional gameplay tag (e.g. "Inventory.Main").
 */
UCLASS()
class GAMEFEATURE_INVENTORYSYSTEMRUNTIME_API UInventorySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	/**
	 * Returns the inventory component for the given owner that matches the tag.
	 * If Tag is valid, returns the first UInventoryManagerComponent on Owner whose InventoryTag equals Tag.
	 * If Tag is invalid, returns the first component implementing IInventoryManagerInterface (backward compatible).
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (WorldContext = "WorldContextObject"))
	static TScriptInterface<IInventoryManagerInterface> GetInventoryForTag(UObject* WorldContextObject, AActor* Owner, FGameplayTag Tag);
};
