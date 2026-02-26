// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "Components/PawnComponent.h"
#include "Interfaces/AbilitySetInterface.h"
#include "Interfaces/EquipmentManagerInterface.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "EquipmentManagerComponent.generated.h"

class UActorComponent;
class UAbilitySystemComponent;
class UEquipmentDefinition;
class UEquipmentInstance;
class UObject;
struct FFrame;
struct FEquipmentList;
struct FNetDeltaSerializeInfo;
struct FReplicationFlags;

/** A single piece of applied equipment */
USTRUCT(BlueprintType)
struct GAMEFEATURE_EQUIPMENTSYSTEMRUNTIME_API FAppliedEquipmentEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FAppliedEquipmentEntry() {}

	FString GetDebugString() const;

private:
	friend FEquipmentList;
	friend UEquipmentManagerComponent;

	UPROPERTY()
	TSubclassOf<UEquipmentDefinition> EquipmentDefinition;

	UPROPERTY()
	TObjectPtr<UEquipmentInstance> Instance = nullptr;

	UPROPERTY(NotReplicated)
	FAbilitySetGrantedHandles GrantedHandles;
};

/** List of applied equipment */
USTRUCT(BlueprintType)
struct GAMEFEATURE_EQUIPMENTSYSTEMRUNTIME_API FEquipmentList : public FFastArraySerializer
{
	GENERATED_BODY()

	FEquipmentList() : OwnerComponent(nullptr) {}
	FEquipmentList(UActorComponent* InOwnerComponent) : OwnerComponent(InOwnerComponent) {}

public:
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FAppliedEquipmentEntry, FEquipmentList>(Entries, DeltaParms, *this);
	}

	UEquipmentInstance* AddEntry(TSubclassOf<UEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(UEquipmentInstance* Instance);

private:
	UAbilitySystemComponent* GetAbilitySystemComponent() const;
	friend UEquipmentManagerComponent;

	UPROPERTY()
	TArray<FAppliedEquipmentEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<> struct TStructOpsTypeTraits<FEquipmentList> : public TStructOpsTypeTraitsBase2<FEquipmentList>
{
	enum { WithNetDeltaSerializer = true };
};

/**
 * Manages equipment applied to a pawn.
 * Implements IEquipmentManagerInterface so the pawn can be queried by interface.
 */
UCLASS(BlueprintType, Const)
class GAMEFEATURE_EQUIPMENTSYSTEMRUNTIME_API UEquipmentManagerComponent : public UPawnComponent, public IEquipmentManagerInterface
{
	GENERATED_BODY()

public:
	UEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	UEquipmentInstance* EquipItem(TSubclassOf<UEquipmentDefinition> EquipmentDefinition);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void UnequipItem(UEquipmentInstance* ItemInstance);

	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
	virtual void ReadyForReplication() override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UEquipmentInstance* GetFirstInstanceOfType(TSubclassOf<UEquipmentInstance> InstanceType);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UEquipmentInstance*> GetEquipmentInstancesOfType(TSubclassOf<UEquipmentInstance> InstanceType) const;

	template <typename T>
	T* GetFirstInstanceOfType() { return (T*)GetFirstInstanceOfType(T::StaticClass()); }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// IEquipmentManagerInterface
	virtual UActorComponent* GetEquipmentManagerComponent_Implementation() const override { return const_cast<UEquipmentManagerComponent*>(this); }

private:
	UPROPERTY(Replicated)
	FEquipmentList EquipmentList;
};
