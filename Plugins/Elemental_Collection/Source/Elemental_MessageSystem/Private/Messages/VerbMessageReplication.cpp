// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "Messages/VerbMessageReplication.h"

#include "GameFramework/GameplayMessageSubsystem.h"
#include "Messages/VerbMessage.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(VerbMessageReplication)

//////////////////////////////////////////////////////////////////////
// FWarCollectionVerbMessageReplicationEntry

FString FVerbMessageReplicationEntry::GetDebugString() const
{
	return Message.ToString();
}

//////////////////////////////////////////////////////////////////////
// FWarCollectionVerbMessageReplication

void FVerbMessageReplication::AddMessage(const FVerbMessage& Message)
{
	FVerbMessageReplicationEntry& NewStack = CurrentMessages.Emplace_GetRef(Message);
	MarkItemDirty(NewStack);
}

void FVerbMessageReplication::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	// Nothing to do here
}

void FVerbMessageReplication::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		const FVerbMessageReplicationEntry& Entry = CurrentMessages[Index];
		RebroadcastMessage(Entry.Message);
	}
}

void FVerbMessageReplication::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (int32 Index : ChangedIndices)
	{
		const FVerbMessageReplicationEntry& Entry = CurrentMessages[Index];
		RebroadcastMessage(Entry.Message);
	}
}

void FVerbMessageReplication::RebroadcastMessage(const FVerbMessage& Message)
{
	check(Owner);
	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(Owner);
	MessageSystem.BroadcastMessage(Message.Verb, Message);
}
