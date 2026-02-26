// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "Components/ControllerComponent.h"
#include "Interfaces/InventoryItemInterface.h"
#include "Interfaces/EquippableItemInterface.h"
#include "Interfaces/EquipmentManagerInterface.h"
#include "QuickBarComponent.generated.h"

class AActor;
class UEquipmentInstance;
class UActorComponent;
class UObject;
struct FFrame;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class GAMEFEATURE_EQUIPMENTSYSTEMRUNTIME_API UQuickBarComponent : public UControllerComponent
{
	GENERATED_BODY()

public:
	UQuickBarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category="WarCollection")
	void CycleActiveSlotForward();

	UFUNCTION(BlueprintCallable, Category="WarCollection")
	void CycleActiveSlotBackward();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="WarCollection")
	void SetActiveSlotIndex(int32 NewIndex);

	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	TArray<UObject*> GetSlots() const { return Slots; }

	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	int32 GetActiveSlotIndex() const { return ActiveSlotIndex; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	UObject* GetActiveSlotItem() const;

	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	int32 GetNextFreeItemSlot() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void AddItemToSlot(int32 SlotIndex, UObject* Item);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	UObject* RemoveItemFromSlot(int32 SlotIndex);

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	void UnequipItemInSlot();
	void EquipItemInSlot();

	class UEquipmentManagerComponent* FindEquipmentManager() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "QuickBar")
	int32 NumSlots = 3;

	UFUNCTION() void OnRep_Slots();
	UFUNCTION() void OnRep_ActiveSlotIndex();

private:
	UPROPERTY(ReplicatedUsing=OnRep_Slots)
	TArray<TObjectPtr<UObject>> Slots;

	UPROPERTY(ReplicatedUsing=OnRep_ActiveSlotIndex)
	int32 ActiveSlotIndex = -1;

	UPROPERTY()
	TObjectPtr<UEquipmentInstance> EquippedItem;
};

USTRUCT(BlueprintType)
struct GAMEFEATURE_EQUIPMENTSYSTEMRUNTIME_API FQuickBarSlotsChangedMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	TObjectPtr<AActor> Owner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	TArray<TObjectPtr<UObject>> Slots;
};

USTRUCT(BlueprintType)
struct GAMEFEATURE_EQUIPMENTSYSTEMRUNTIME_API FQuickBarActiveIndexChangedMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	TObjectPtr<AActor> Owner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	int32 ActiveIndex = 0;
};
