// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "GameplayTagContainer.h"
#include "Messages/VerbMessage.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "VerbMessageReplication.generated.h"

class UObject;
struct FVerbMessageReplication;
struct FNetDeltaSerializeInfo;

/**
 * Represents one verb message
 */
USTRUCT(BlueprintType)
struct ELEMENTAL_MESSAGESYSTEM_API FVerbMessageReplicationEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FVerbMessageReplicationEntry()
	{}

	FVerbMessageReplicationEntry(const FVerbMessage& InMessage)
		: Message(InMessage)
	{
	}

	FString GetDebugString() const;

private:
	friend FVerbMessageReplication;

	UPROPERTY()
	FVerbMessage Message;
};

/** Container of verb messages to replicate */
USTRUCT(BlueprintType)
struct ELEMENTAL_MESSAGESYSTEM_API FVerbMessageReplication : public FFastArraySerializer
{
	GENERATED_BODY()

	FVerbMessageReplication()
	{
	}

public:
	void SetOwner(UObject* InOwner) { Owner = InOwner; }

	// Broadcasts a message from server to clients
	void AddMessage(const FVerbMessage& Message);

	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FVerbMessageReplicationEntry, FVerbMessageReplication>(CurrentMessages, DeltaParms, *this);
	}

private:
	void RebroadcastMessage(const FVerbMessage& Message);

private:
	// Replicated list of gameplay tag stacks
	UPROPERTY()
	TArray<FVerbMessageReplicationEntry> CurrentMessages;
	
	// Owner (for a route to a world)
	UPROPERTY()
	TObjectPtr<UObject> Owner = nullptr;
};

template<>
struct TStructOpsTypeTraits<FVerbMessageReplication> : public TStructOpsTypeTraitsBase2<FVerbMessageReplication>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};
