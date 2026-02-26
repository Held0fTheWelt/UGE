// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "InventoryItemDefinition.h"
#include "Interfaces/InventoryItemDefinitionInterface.h"
#include "InventoryItemInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InventoryItemDefinition)

UInventoryItemDefinition::UInventoryItemDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const UObject* UInventoryItemDefinition::FindFragmentByClass(TSubclassOf<UObject> FragmentClass) const
{
	if (FragmentClass != nullptr)
	{
		for (const TObjectPtr<UObject>& Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}

	return nullptr;
}

//////////////////////////////////////////////////////////////////////
// UInventoryItemFragment

void UInventoryItemFragment::OnInstanceCreated(UObject* ItemInstance) const
{
	if (UInventoryItemInstance* Instance = Cast<UInventoryItemInstance>(ItemInstance))
	{
		OnInstanceCreatedForSubclass(Instance);
	}
}

//////////////////////////////////////////////////////////////////////
// UInventoryFunctionLibrary

const UObject* UInventoryFunctionLibrary::FindItemDefinitionFragment(TSubclassOf<UObject> ItemDef, TSubclassOf<UObject> FragmentClass)
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		if (const IInventoryItemDefinitionInterface* DefInterface = Cast<IInventoryItemDefinitionInterface>(GetDefault<UObject>(ItemDef)))
		{
			return DefInterface->FindFragmentByClass(FragmentClass);
		}
	}
	return nullptr;
}
