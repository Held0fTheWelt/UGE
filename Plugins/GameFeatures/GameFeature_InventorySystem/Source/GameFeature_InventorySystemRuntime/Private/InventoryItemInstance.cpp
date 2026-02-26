// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "InventoryItemInstance.h"
#include "InventoryItemDefinition.h"
#include "System/GameplayTagStack.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InventoryItemInstance)

UInventoryItemInstance::UInventoryItemInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ItemDef = nullptr;
}

void UInventoryItemInstance::AddStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.AddStack(Tag, StackCount);
}

void UInventoryItemInstance::RemoveStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.RemoveStack(Tag, StackCount);
}

int32 UInventoryItemInstance::GetStatTagStackCount(FGameplayTag Tag) const
{
	return StatTags.GetStackCount(Tag);
}

bool UInventoryItemInstance::HasStatTag(FGameplayTag Tag) const
{
	return StatTags.ContainsTag(Tag);
}

const UObject* UInventoryItemInstance::FindFragmentByClass_Implementation(TSubclassOf<UObject> FragmentClass) const
{
	if ((FragmentClass != nullptr) && (ItemDef != nullptr))
	{
		const UInventoryItemDefinition* Def = GetDefault<UInventoryItemDefinition>(ItemDef);
		if (const IInventoryItemDefinitionInterface* DefInterface = Cast<IInventoryItemDefinitionInterface>(Def))
		{
			return DefInterface->FindFragmentByClass(FragmentClass);
		}
	}

	return nullptr;
}

void UInventoryItemInstance::SetItemDef(TSubclassOf<UInventoryItemDefinition> InDef)
{
	ItemDef = InDef;
}

TSubclassOf<UObject> UInventoryItemInstance::GetItemDefinitionClass_Implementation() const
{
	return ItemDef;
}

void UInventoryItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryItemInstance, StatTags);
	DOREPLIFETIME(UInventoryItemInstance, ItemDef);
}
