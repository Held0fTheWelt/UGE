// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "QuickBar/QuickBarComponent.h"
#include "Equipment/EquipmentDefinition.h"
#include "Equipment/EquipmentInstance.h"
#include "Equipment/EquipmentManagerComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameFramework/Pawn.h"
#include "GameplayTagContainer.h"
#include "Interfaces/EquippableItemInterface.h"
#include "Interfaces/EquipmentManagerInterface.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(QuickBarComponent)

class FLifetimeProperty;

namespace
{
	const FGameplayTag TAG_QuickBar_Message_SlotsChanged = FGameplayTag::RequestGameplayTag(FName("QuickBar.Message.SlotsChanged"), false);
	const FGameplayTag TAG_QuickBar_Message_ActiveIndexChanged = FGameplayTag::RequestGameplayTag(FName("QuickBar.Message.ActiveIndexChanged"), false);
}

UQuickBarComponent::UQuickBarComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}

void UQuickBarComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, Slots);
	DOREPLIFETIME(ThisClass, ActiveSlotIndex);
}

void UQuickBarComponent::BeginPlay()
{
	if (Slots.Num() < NumSlots)
	{
		Slots.AddDefaulted(NumSlots - Slots.Num());
	}
	Super::BeginPlay();
}

void UQuickBarComponent::CycleActiveSlotForward()
{
	if (Slots.Num() < 2) return;

	const int32 OldIndex = (ActiveSlotIndex < 0 ? Slots.Num()-1 : ActiveSlotIndex);
	int32 NewIndex = ActiveSlotIndex;
	do
	{
		NewIndex = (NewIndex + 1) % Slots.Num();
		if (Slots[NewIndex] != nullptr)
		{
			SetActiveSlotIndex(NewIndex);
			return;
		}
	} while (NewIndex != OldIndex);
}

void UQuickBarComponent::CycleActiveSlotBackward()
{
	if (Slots.Num() < 2) return;

	const int32 OldIndex = (ActiveSlotIndex < 0 ? Slots.Num()-1 : ActiveSlotIndex);
	int32 NewIndex = ActiveSlotIndex;
	do
	{
		NewIndex = (NewIndex - 1 + Slots.Num()) % Slots.Num();
		if (Slots[NewIndex] != nullptr)
		{
			SetActiveSlotIndex(NewIndex);
			return;
		}
	} while (NewIndex != OldIndex);
}

void UQuickBarComponent::EquipItemInSlot()
{
	check(Slots.IsValidIndex(ActiveSlotIndex));
	check(EquippedItem == nullptr);

	UObject* SlotItem = Slots[ActiveSlotIndex];
	if (!SlotItem) return;

	TSubclassOf<UObject> EquipDefClass;
	if (IEquippableItemInterface* Equippable = Cast<IEquippableItemInterface>(SlotItem))
	{
		EquipDefClass = Equippable->GetEquipmentDefinitionClass();
	}

	if (EquipDefClass && EquipDefClass->IsChildOf(UEquipmentDefinition::StaticClass()))
	{
		if (UEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
		{
			EquippedItem = EquipmentManager->EquipItem(TSubclassOf<UEquipmentDefinition>(EquipDefClass));
			if (EquippedItem)
			{
				EquippedItem->SetInstigator(SlotItem);
			}
		}
	}
}

void UQuickBarComponent::UnequipItemInSlot()
{
	if (UEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
	{
		if (EquippedItem)
		{
			EquipmentManager->UnequipItem(EquippedItem);
			EquippedItem = nullptr;
		}
	}
}

UEquipmentManagerComponent* UQuickBarComponent::FindEquipmentManager() const
{
	if (AController* OwnerController = Cast<AController>(GetOwner()))
	{
		if (APawn* Pawn = OwnerController->GetPawn())
		{
			if (IEquipmentManagerInterface* ManagerInterface = Cast<IEquipmentManagerInterface>(Pawn))
			{
				return Cast<UEquipmentManagerComponent>(ManagerInterface->GetEquipmentManagerComponent());
			}
			return Pawn->FindComponentByClass<UEquipmentManagerComponent>();
		}
	}
	return nullptr;
}

void UQuickBarComponent::SetActiveSlotIndex_Implementation(int32 NewIndex)
{
	if (Slots.IsValidIndex(NewIndex) && (ActiveSlotIndex != NewIndex))
	{
		UnequipItemInSlot();
		ActiveSlotIndex = NewIndex;
		EquipItemInSlot();
		OnRep_ActiveSlotIndex();
	}
}

UObject* UQuickBarComponent::GetActiveSlotItem() const
{
	return Slots.IsValidIndex(ActiveSlotIndex) ? Slots[ActiveSlotIndex] : nullptr;
}

int32 UQuickBarComponent::GetNextFreeItemSlot() const
{
	int32 SlotIndex = 0;
	for (const TObjectPtr<UObject>& ItemPtr : Slots)
	{
		if (ItemPtr == nullptr) return SlotIndex;
		++SlotIndex;
	}
	return INDEX_NONE;
}

void UQuickBarComponent::AddItemToSlot(int32 SlotIndex, UObject* Item)
{
	if (Slots.IsValidIndex(SlotIndex) && Item != nullptr)
	{
		if (Slots[SlotIndex] == nullptr)
		{
			Slots[SlotIndex] = Item;
			OnRep_Slots();
		}
	}
}

UObject* UQuickBarComponent::RemoveItemFromSlot(int32 SlotIndex)
{
	UObject* Result = nullptr;

	if (ActiveSlotIndex == SlotIndex)
	{
		UnequipItemInSlot();
		ActiveSlotIndex = -1;
	}

	if (Slots.IsValidIndex(SlotIndex))
	{
		Result = Slots[SlotIndex];
		if (Result != nullptr)
		{
			Slots[SlotIndex] = nullptr;
			OnRep_Slots();
		}
	}
	return Result;
}

void UQuickBarComponent::OnRep_Slots()
{
	FQuickBarSlotsChangedMessage Message;
	Message.Owner = GetOwner();
	Message.Slots = Slots;
	UGameplayMessageSubsystem::Get(this).BroadcastMessage(TAG_QuickBar_Message_SlotsChanged, Message);
}

void UQuickBarComponent::OnRep_ActiveSlotIndex()
{
	FQuickBarActiveIndexChangedMessage Message;
	Message.Owner = GetOwner();
	Message.ActiveIndex = ActiveSlotIndex;
	UGameplayMessageSubsystem::Get(this).BroadcastMessage(TAG_QuickBar_Message_ActiveIndexChanged, Message);
}
