// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Interfaces/InventoryItemFragmentInterface.h"
#include "Interfaces/InventoryItemDefinitionInterface.h"
#include "InventoryItemDefinition.generated.h"

template <typename T> class TSubclassOf;
class UInventoryItemInstance;
class UObject;

/** Base fragment class for this GameFeature; implements IInventoryItemFragmentInterface so other GameFeatures can add fragments via interface. */
UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class GAMEFEATURE_INVENTORYSYSTEMRUNTIME_API UInventoryItemFragment : public UObject, public IInventoryItemFragmentInterface
{
	GENERATED_BODY()

public:
	//~IInventoryItemFragmentInterface
	void OnInstanceCreated(UObject* ItemInstance) const override;

	/** Override in subclasses; called from OnInstanceCreated after casting ItemInstance to UInventoryItemInstance. */
	virtual void OnInstanceCreatedForSubclass(UInventoryItemInstance* Instance) const {}
};

/**
 * UInventoryItemDefinition
 *
 *	Definition of an inventory item. Implements IInventoryItemDefinitionInterface so other GameFeatures can query fragments without depending on this module.
 *	Adapted from ULyraInventoryItemDefinition.
 */
UCLASS(Blueprintable, Const, Abstract)
class GAMEFEATURE_INVENTORYSYSTEMRUNTIME_API UInventoryItemDefinition : public UObject, public IInventoryItemDefinitionInterface
{
	GENERATED_BODY()

public:
	UInventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display)
	FText DisplayName;

	/** Max total stack count per inventory (0 = no limit). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Inventory, meta=(ClampMin="0"))
	int32 MaxStackCount = 0;

	/** If true, only one stack/entry of this definition is allowed per inventory. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Inventory)
	bool bUnique = false;

	/** Fragments must implement IInventoryItemFragmentInterface (e.g. UInventoryItemFragment, or fragment types from other GameFeatures). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display, Instanced, meta = (MustImplement = "/Script/Elemental_Interfaces.InventoryItemFragmentInterface"))
	TArray<TObjectPtr<UObject>> Fragments;

public:
	//~IInventoryItemDefinitionInterface
	const UObject* FindFragmentByClass(TSubclassOf<UObject> FragmentClass) const override;
};

//@TODO: Make into a subsystem instead?
UCLASS()
class GAMEFEATURE_INVENTORYSYSTEMRUNTIME_API UInventoryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/** Returns the first fragment of FragmentClass from the item definition (ItemDef must implement IInventoryItemDefinitionInterface). */
	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType=FragmentClass))
	static const UObject* FindItemDefinitionFragment(TSubclassOf<UObject> ItemDef, TSubclassOf<UObject> FragmentClass);
};
