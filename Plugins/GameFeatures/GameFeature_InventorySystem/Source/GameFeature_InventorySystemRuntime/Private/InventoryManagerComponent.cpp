// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "InventoryManagerComponent.h"
#include "InventoryItemDefinition.h"
#include "InventoryItemInstance.h"
#include "Interfaces/InventoryItemFragmentInterface.h"
#include "Interfaces/InventoryItemInstanceInterface.h"
#include "Engine/ActorChannel.h"
#include "Engine/World.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameplayTagContainer.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InventoryManagerComponent)

class FLifetimeProperty;
struct FReplicationFlags;

namespace
{
	const FName InventoryMessageStackChangedName("Inventory.Message.StackChanged");
}

//////////////////////////////////////////////////////////////////////
// FInventoryEntry

FString FInventoryEntry::GetDebugString() const
{
	TSubclassOf<UInventoryItemDefinition> ItemDef;
	if (Instance != nullptr)
	{
		ItemDef = Instance->GetItemDef();
	}

	return FString::Printf(TEXT("%s (%d x %s)"), *GetNameSafe(Instance), StackCount, *GetNameSafe(ItemDef));
}

//////////////////////////////////////////////////////////////////////
// FInventoryList

void FInventoryList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		FInventoryEntry& Stack = Entries[Index];
		BroadcastChangeMessage(Stack, /*OldCount=*/ Stack.StackCount, /*NewCount=*/ 0);
		Stack.LastObservedCount = 0;
	}
}

void FInventoryList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		FInventoryEntry& Stack = Entries[Index];
		BroadcastChangeMessage(Stack, /*OldCount=*/ 0, /*NewCount=*/ Stack.StackCount);
		Stack.LastObservedCount = Stack.StackCount;
	}
}

void FInventoryList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (int32 Index : ChangedIndices)
	{
		FInventoryEntry& Stack = Entries[Index];
		check(Stack.LastObservedCount != INDEX_NONE);
		BroadcastChangeMessage(Stack, /*OldCount=*/ Stack.LastObservedCount, /*NewCount=*/ Stack.StackCount);
		Stack.LastObservedCount = Stack.StackCount;
	}
}

void FInventoryList::BroadcastChangeMessage(FInventoryEntry& Entry, int32 OldCount, int32 NewCount)
{
	FInventoryChangeMessage Message;
	Message.InventoryOwner = OwnerComponent;
	Message.Instance = Entry.Instance;
	Message.NewCount = NewCount;
	Message.Delta = NewCount - OldCount;

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(OwnerComponent->GetWorld());
	MessageSystem.BroadcastMessage(FGameplayTag::RequestGameplayTag(InventoryMessageStackChangedName), Message);
}

UInventoryItemInstance* FInventoryList::AddEntry(TSubclassOf<UInventoryItemDefinition> ItemDef, int32 StackCount)
{
	UInventoryItemInstance* Result = nullptr;

	check(ItemDef != nullptr);
 	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());

	FInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<UInventoryItemInstance>(OwnerComponent->GetOwner());  // Outer: actor used due to UE-127172; use OwnerComponent when fixed
	NewEntry.Instance->SetItemDef(ItemDef);
	
	const UInventoryItemDefinition* ItemCDO = GetDefault<UInventoryItemDefinition>(ItemDef);
	for (UObject* Fragment : ItemCDO->Fragments)
	{
		if (IInventoryItemFragmentInterface* IFrag = Cast<IInventoryItemFragmentInterface>(Fragment))
		{
			IFrag->OnInstanceCreated(NewEntry.Instance);
		}
	}
	
	NewEntry.StackCount = StackCount;
	Result = NewEntry.Instance;

	MarkItemDirty(NewEntry);

	return Result;
}

void FInventoryList::AddEntry(UInventoryItemInstance* Instance)
{
	check(Instance != nullptr);
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());

	FInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = Instance;
	NewEntry.StackCount = 1;

	const TSubclassOf<UInventoryItemDefinition> ItemDef = Instance->GetItemDef();
	if (ItemDef)
	{
		const UInventoryItemDefinition* ItemCDO = GetDefault<UInventoryItemDefinition>(ItemDef);
		for (UObject* Fragment : ItemCDO->Fragments)
		{
			if (IInventoryItemFragmentInterface* IFrag = Cast<IInventoryItemFragmentInterface>(Fragment))
			{
				IFrag->OnInstanceCreated(Instance);
			}
		}
	}

	MarkItemDirty(NewEntry);
}

void FInventoryList::RemoveEntry(UInventoryItemInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FInventoryEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

TArray<UInventoryItemInstance*> FInventoryList::GetAllItems() const
{
	TArray<UInventoryItemInstance*> Results;
	Results.Reserve(Entries.Num());
	for (const FInventoryEntry& Entry : Entries)
	{
		if (Entry.Instance != nullptr) //@TODO: Would prefer to not deal with this here and hide it further?
		{
			Results.Add(Entry.Instance);
		}
	}
	return Results;
}

//////////////////////////////////////////////////////////////////////
// UInventoryManagerComponent

UInventoryManagerComponent::UInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, InventoryList(this)
{
	SetIsReplicatedByDefault(true);
}

void UInventoryManagerComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}

bool UInventoryManagerComponent::CanAddItemDefinition(TSubclassOf<UObject> ItemDefClass, int32 StackCount) const
{
	return CanAddItemDefinitionTyped(TSubclassOf<UInventoryItemDefinition>(ItemDefClass), StackCount);
}

UObject* UInventoryManagerComponent::AddItemDefinition(TSubclassOf<UObject> ItemDefClass, int32 StackCount)
{
	return AddItemDefinitionTyped(TSubclassOf<UInventoryItemDefinition>(ItemDefClass), StackCount);
}

UObject* UInventoryManagerComponent::FindFirstItemStackByDefinition(TSubclassOf<UObject> ItemDefClass) const
{
	return FindFirstItemStackByDefinitionTyped(TSubclassOf<UInventoryItemDefinition>(ItemDefClass));
}

int32 UInventoryManagerComponent::GetTotalItemCountByDefinition(TSubclassOf<UObject> ItemDefClass) const
{
	return GetTotalItemCountByDefinitionTyped(TSubclassOf<UInventoryItemDefinition>(ItemDefClass));
}

bool UInventoryManagerComponent::CanAddItemDefinitionTyped(TSubclassOf<UInventoryItemDefinition> ItemDef, int32 StackCount) const
{
	if (!ItemDef) { return false; }
	const UInventoryItemDefinition* ItemCDO = GetDefault<UInventoryItemDefinition>(ItemDef);
	const int32 CurrentCount = GetTotalItemCountByDefinitionTyped(ItemDef);
	if (ItemCDO->bUnique && CurrentCount > 0) { return false; }
	if (ItemCDO->MaxStackCount > 0 && (CurrentCount + StackCount) > ItemCDO->MaxStackCount) { return false; }
	return true;
}

UInventoryItemInstance* UInventoryManagerComponent::AddItemDefinitionTyped(TSubclassOf<UInventoryItemDefinition> ItemDef, int32 StackCount)
{
	UInventoryItemInstance* Result = nullptr;
	if (ItemDef != nullptr && CanAddItemDefinitionTyped(ItemDef, StackCount))
	{
		Result = InventoryList.AddEntry(ItemDef, StackCount);
		
		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && Result)
		{
			AddReplicatedSubObject(Result);
		}
	}
	return Result;
}

void UInventoryManagerComponent::AddItemInstance(UInventoryItemInstance* ItemInstance)
{
	InventoryList.AddEntry(ItemInstance);
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && ItemInstance)
	{
		AddReplicatedSubObject(ItemInstance);
	}
}

void UInventoryManagerComponent::RemoveItemInstance(UInventoryItemInstance* ItemInstance)
{
	InventoryList.RemoveEntry(ItemInstance);

	if (ItemInstance && IsUsingRegisteredSubObjectList())
	{
		RemoveReplicatedSubObject(ItemInstance);
	}
}

TArray<UInventoryItemInstance*> UInventoryManagerComponent::GetAllItems() const
{
	return InventoryList.GetAllItems();
}

UInventoryItemInstance* UInventoryManagerComponent::FindFirstItemStackByDefinitionTyped(TSubclassOf<UInventoryItemDefinition> ItemDef) const
{
	for (const FInventoryEntry& Entry : InventoryList.Entries)
	{
		UInventoryItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			if (Instance->GetItemDef() == ItemDef)
			{
				return Instance;
			}
		}
	}

	return nullptr;
}

int32 UInventoryManagerComponent::GetTotalItemCountByDefinitionTyped(TSubclassOf<UInventoryItemDefinition> ItemDef) const
{
	int32 TotalCount = 0;
	for (const FInventoryEntry& Entry : InventoryList.Entries)
	{
		UInventoryItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			if (Instance->GetItemDef() == ItemDef)
			{
				TotalCount += Entry.StackCount;
			}
		}
	}

	return TotalCount;
}

bool UInventoryManagerComponent::ConsumeItemsByDefinition(TSubclassOf<UInventoryItemDefinition> ItemDef, int32 NumToConsume)
{
	AActor* OwningActor = GetOwner();
	if (!OwningActor || !OwningActor->HasAuthority())
	{
		return false;
	}

	//@TODO: N squared right now as there's no acceleration structure
	int32 NumRemainingToConsume = NumToConsume;
	for (FInventoryEntry& Entry : InventoryList.Entries)
	{
		UInventoryItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			if (Instance->GetItemDef() == ItemDef)
			{
				const int32 NumToConsumeFromThisStack = FMath::Min(NumRemainingToConsume, Entry.StackCount);
				Entry.StackCount -= NumToConsumeFromThisStack;
				NumRemainingToConsume -= NumToConsumeFromThisStack;

				InventoryList.MarkItemDirty(Entry);

				if (Entry.StackCount <= 0)
				{
					RemoveItemInstance(Instance);
				}

				if (NumRemainingToConsume <= 0)
				{
					return true;
				}
			}
		}
	}

	return (NumRemainingToConsume == 0);
}

bool UInventoryManagerComponent::ReplicateSubobjects(UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FInventoryEntry& Entry : InventoryList.Entries)
	{
		UInventoryItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void UInventoryManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	// Register existing inventory item instances
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FInventoryEntry& Entry : InventoryList.Entries)
		{
			UInventoryItemInstance* Instance = Entry.Instance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
// IInventoryManagerInterface

TArray<UObject*> UInventoryManagerComponent::GetInventoryItems() const
{
	TArray<UInventoryItemInstance*> Items = GetAllItems();
	TArray<UObject*> Result;
	Result.Reserve(Items.Num());
	for (UInventoryItemInstance* Item : Items)
	{
		Result.Add(Item);
	}
	return Result;
}
