// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "Interfaces/InventoryManagerInterface.h"
#include "Interfaces/InventoryItemInstanceInterface.h"
#include "GameplayTagContainer.h"
#include "InventoryManagerComponent.generated.h"

class UInventoryItemDefinition;
class UInventoryItemInstance;
class UObject;
struct FFrame;
struct FInventoryList;
struct FNetDeltaSerializeInfo;
struct FReplicationFlags;

/** A message when an item is added to the inventory */
USTRUCT(BlueprintType)
struct GAMEFEATURE_INVENTORYSYSTEMRUNTIME_API FInventoryChangeMessage
{
	GENERATED_BODY()

	/** Owner component (supports tag-based lookup via UInventorySubsystem::GetInventoryForTag). */
	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	TObjectPtr<UActorComponent> InventoryOwner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	TObjectPtr<UInventoryItemInstance> Instance = nullptr;

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	int32 NewCount = 0;

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	int32 Delta = 0;
};

/** A single entry in an inventory */
USTRUCT(BlueprintType)
struct GAMEFEATURE_INVENTORYSYSTEMRUNTIME_API FInventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FInventoryEntry()
	{}

	FString GetDebugString() const;

private:
	friend FInventoryList;
	friend UInventoryManagerComponent;

	UPROPERTY()
	TObjectPtr<UInventoryItemInstance> Instance = nullptr;

	UPROPERTY()
	int32 StackCount = 0;

	UPROPERTY(NotReplicated)
	int32 LastObservedCount = INDEX_NONE;
};

/** List of inventory items */
USTRUCT(BlueprintType)
struct GAMEFEATURE_INVENTORYSYSTEMRUNTIME_API FInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

	FInventoryList()
		: OwnerComponent(nullptr)
	{
	}

	FInventoryList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

	TArray<UInventoryItemInstance*> GetAllItems() const;

public:
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FInventoryEntry, FInventoryList>(Entries, DeltaParms, *this);
	}

	UInventoryItemInstance* AddEntry(TSubclassOf<UInventoryItemDefinition> ItemClass, int32 StackCount);
	void AddEntry(UInventoryItemInstance* Instance);

	void RemoveEntry(UInventoryItemInstance* Instance);

private:
	void BroadcastChangeMessage(FInventoryEntry& Entry, int32 OldCount, int32 NewCount);

private:
	friend UInventoryManagerComponent;

private:
	// Replicated list of items
	UPROPERTY()
	TArray<FInventoryEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FInventoryList> : public TStructOpsTypeTraitsBase2<FInventoryList>
{
	enum { WithNetDeltaSerializer = true };
};

/**
 * UInventoryManagerComponent
 *
 *	Component that manages inventory for a pawn.
 *	Adapted from ULyraInventoryManagerComponent.
 */
UCLASS(MinimalAPI, BlueprintType, Meta=(BlueprintSpawnableComponent))
class UInventoryManagerComponent : public UPawnComponent, public IInventoryManagerInterface
{
	GENERATED_BODY()

public:
	UInventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~IInventoryManagerInterface (override with TSubclassOf<UObject> for interface contract)
	UFUNCTION(BlueprintCallable, Category = Inventory)
	GAMEFEATURE_INVENTORYSYSTEMRUNTIME_API TArray<UObject*> GetInventoryItems() const override;
	UFUNCTION(BlueprintCallable, Category = Inventory)
	GAMEFEATURE_INVENTORYSYSTEMRUNTIME_API bool CanAddItemDefinition(TSubclassOf<UObject> ItemDefClass, int32 StackCount) const override;
	UFUNCTION(BlueprintCallable, Category = Inventory)
	GAMEFEATURE_INVENTORYSYSTEMRUNTIME_API UObject* AddItemDefinition(TSubclassOf<UObject> ItemDefClass, int32 StackCount) override;
	UFUNCTION(BlueprintCallable, Category = Inventory)
	GAMEFEATURE_INVENTORYSYSTEMRUNTIME_API UObject* FindFirstItemStackByDefinition(TSubclassOf<UObject> ItemDefClass) const override;
	UFUNCTION(BlueprintCallable, Category = Inventory)
	GAMEFEATURE_INVENTORYSYSTEMRUNTIME_API int32 GetTotalItemCountByDefinition(TSubclassOf<UObject> ItemDefClass) const override;

	/** Strongly-typed overloads for C++ (delegate to interface overrides). */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	GAMEFEATURE_INVENTORYSYSTEMRUNTIME_API bool CanAddItemDefinitionTyped(TSubclassOf<UInventoryItemDefinition> ItemDef, int32 StackCount = 1) const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	GAMEFEATURE_INVENTORYSYSTEMRUNTIME_API UInventoryItemInstance* AddItemDefinitionTyped(TSubclassOf<UInventoryItemDefinition> ItemDef, int32 StackCount = 1);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	GAMEFEATURE_INVENTORYSYSTEMRUNTIME_API void AddItemInstance(UInventoryItemInstance* ItemInstance);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	GAMEFEATURE_INVENTORYSYSTEMRUNTIME_API void RemoveItemInstance(UInventoryItemInstance* ItemInstance);

	UFUNCTION(BlueprintCallable, Category=Inventory, BlueprintPure=false)
	GAMEFEATURE_INVENTORYSYSTEMRUNTIME_API TArray<UInventoryItemInstance*> GetAllItems() const;

	UFUNCTION(BlueprintCallable, Category=Inventory, BlueprintPure)
	GAMEFEATURE_INVENTORYSYSTEMRUNTIME_API UInventoryItemInstance* FindFirstItemStackByDefinitionTyped(TSubclassOf<UInventoryItemDefinition> ItemDef) const;

	GAMEFEATURE_INVENTORYSYSTEMRUNTIME_API int32 GetTotalItemCountByDefinitionTyped(TSubclassOf<UInventoryItemDefinition> ItemDef) const;
	GAMEFEATURE_INVENTORYSYSTEMRUNTIME_API bool ConsumeItemsByDefinition(TSubclassOf<UInventoryItemDefinition> ItemDef, int32 NumToConsume);

	/** Optional tag to identify this inventory (e.g. "Inventory.Main", "Inventory.QuickBar"). Used by subsystem lookup. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Inventory, meta = (Categories = "Inventory"))
	FGameplayTag InventoryTag;

	/** Returns the inventory tag set on this component. */
	UFUNCTION(BlueprintPure, Category = Inventory)
	FGameplayTag GetInventoryTag() const { return InventoryTag; }

	//~UObject interface
	GAMEFEATURE_INVENTORYSYSTEMRUNTIME_API virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	GAMEFEATURE_INVENTORYSYSTEMRUNTIME_API virtual void ReadyForReplication() override;
	//~End of UObject interface

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(Replicated)
	FInventoryList InventoryList;
};
