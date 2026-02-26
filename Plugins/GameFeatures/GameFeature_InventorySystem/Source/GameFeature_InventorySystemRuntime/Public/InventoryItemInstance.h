// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "System/GameplayTagStack.h"
#include "Interfaces/InventoryItemInstanceInterface.h"
#include "Interfaces/InventoryItemDefinitionInterface.h"
#include "InventoryItemInstance.generated.h"

template <typename T> class TSubclassOf;
class UInventoryItemDefinition;
class UObject;
struct FGameplayTag;

/**
 * UInventoryItemInstance
 *
 *	Instance of an inventory item.
 *	Adapted from ULyraInventoryItemInstance.
 */
UCLASS(BlueprintType)
class GAMEFEATURE_INVENTORYSYSTEMRUNTIME_API UInventoryItemInstance : public UObject, public IInventoryItemInstanceInterface
{
	GENERATED_BODY()

public:
	UInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UObject interface
	virtual bool IsSupportedForNetworking() const override { return true; }
	//~End of UObject interface

	// Adds a specified number of stacks to the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	void AddStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Removes a specified number of stacks from the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	void RemoveStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Returns the stack count of the specified tag (or 0 if the tag is not present)
	UFUNCTION(BlueprintCallable, Category=Inventory)
	int32 GetStatTagStackCount(FGameplayTag Tag) const;

	// Returns true if there is at least one stack of the specified tag
	UFUNCTION(BlueprintCallable, Category=Inventory)
	bool HasStatTag(FGameplayTag Tag) const;

	TSubclassOf<UInventoryItemDefinition> GetItemDef() const
	{
		return ItemDef;
	}

	//~IInventoryItemInstanceInterface
	TSubclassOf<UObject> GetItemDefinitionClass_Implementation() const override;
	const UObject* FindFragmentByClass_Implementation(TSubclassOf<UObject> FragmentClass) const override;

	/** Helper: returns the first fragment of the given class (use IInventoryItemInstanceInterface::Execute_FindFragmentByClass or this template from C++). */
	template <typename ResultClass>
	const ResultClass* FindFragmentByClass() const
	{
		return (const ResultClass*)FindFragmentByClass_Implementation(ResultClass::StaticClass());
	}

private:
	void SetItemDef(TSubclassOf<UInventoryItemDefinition> InDef);

	friend class UInventoryManagerComponent;
	friend struct FInventoryList;

private:
	UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;

	// The item definition
	UPROPERTY(Replicated)
	TSubclassOf<UInventoryItemDefinition> ItemDef;
};
