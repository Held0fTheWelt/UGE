// Copyright Epic Games, Inc. All Rights Reserved.
// Core_Collection

#pragma once

#include "ReplicationGraph.h"
#include "Replication/CoreReplicationGraphTypes.h"
#include "CoreReplicationGraph.generated.h"

class AGameplayDebuggerCategoryReplicator;

DECLARE_LOG_CATEGORY_EXTERN(LogCoreRepGraph, Display, All);

/** Call from Core_System.cpp to force CoreReplicationGraph.cpp.obj to be linked (defines PlayerStateFrequencyLimiter ctor, FSharedRepMovement usage). */
void CoreReplicationGraphForceLink();

/** Dummy symbol in CoreReplicationGraph.cpp so the linker includes that translation unit. */
void CoreReplicationGraphForceLinkDummy();

/** Core Replication Graph implementation. See additional notes in CoreReplicationGraph.cpp! */
UCLASS(transient, config=Engine)
class CORE_SYSTEM_API UCoreReplicationGraph : public UReplicationGraph
{
	GENERATED_BODY()

public:
	UCoreReplicationGraph();

	virtual void ResetGameWorldState() override;

	virtual void InitGlobalActorClassSettings() override;
	virtual void InitGlobalGraphNodes() override;
	virtual void InitConnectionGraphNodes(UNetReplicationGraphConnection* RepGraphConnection) override;
	virtual void RouteAddNetworkActorToNodes(const FNewReplicatedActorInfo& ActorInfo, FGlobalActorReplicationInfo& GlobalInfo) override;
	virtual void RouteRemoveNetworkActorToNodes(const FNewReplicatedActorInfo& ActorInfo) override;

	UPROPERTY()
	TArray<TObjectPtr<UClass>>	AlwaysRelevantClasses;
	
	UPROPERTY()
	TObjectPtr<UReplicationGraphNode_GridSpatialization2D> GridNode;

	UPROPERTY()
	TObjectPtr<UReplicationGraphNode_ActorList> AlwaysRelevantNode;

	TMap<FName, FActorRepListRefView> AlwaysRelevantStreamingLevelActors;

#if WITH_GAMEPLAY_DEBUGGER
	void OnGameplayDebuggerOwnerChange(AGameplayDebuggerCategoryReplicator* Debugger, APlayerController* OldOwner);
#endif

	void PrintRepNodePolicies();

private:
	void AddClassRepInfo(UClass* Class, ECoreClassRepNodeMapping Mapping);
	void RegisterClassRepNodeMapping(UClass* Class);
	ECoreClassRepNodeMapping GetClassNodeMapping(UClass* Class) const;

	void RegisterClassReplicationInfo(UClass* Class);
	bool ConditionalInitClassReplicationInfo(UClass* Class, FClassReplicationInfo& ClassInfo);
	void InitClassReplicationInfo(FClassReplicationInfo& Info, UClass* Class, bool Spatialize) const;

	ECoreClassRepNodeMapping GetMappingPolicy(UClass* Class);

	bool IsSpatialized(ECoreClassRepNodeMapping Mapping) const { return Mapping >= ECoreClassRepNodeMapping::Spatialize_Static; }

	TClassMap<ECoreClassRepNodeMapping> ClassRepNodePolicies;

	/** Classes that had their replication settings explictly set by code in UCoreReplicationGraph::InitGlobalActorClassSettings */
	TArray<UClass*> ExplicitlySetClasses;
};

UCLASS()
class CORE_SYSTEM_API UCoreReplicationGraphNode_AlwaysRelevant_ForConnection : public UReplicationGraphNode_AlwaysRelevant_ForConnection
{
	GENERATED_BODY()

public:
	virtual void NotifyAddNetworkActor(const FNewReplicatedActorInfo& Actor) override { }
	virtual bool NotifyRemoveNetworkActor(const FNewReplicatedActorInfo& ActorInfo, bool bWarnIfNotFound=true) override { return false; }
	virtual void NotifyResetAllNetworkActors() override { }

	virtual void GatherActorListsForConnection(const FConnectionGatherActorListParameters& Params) override;

	virtual void LogNode(FReplicationGraphDebugInfo& DebugInfo, const FString& NodeName) const override;

	void OnClientLevelVisibilityAdd(FName LevelName, UWorld* StreamingWorld);
	void OnClientLevelVisibilityRemove(FName LevelName);

	void ResetGameWorldState();

#if WITH_GAMEPLAY_DEBUGGER
	AGameplayDebuggerCategoryReplicator* GameplayDebugger = nullptr;
#endif

private:
	TArray<FName, TInlineAllocator<64> > AlwaysRelevantStreamingLevelsNeedingReplication;

	bool bInitializedPlayerState = false;
};

/** 
	This is a specialized node for handling PlayerState replication in a frequency limited fashion. It tracks all player states but only returns a subset of them to the replication driver each frame. 
	This is an optimization for large player connection counts, and not a requirement.
*/
UCLASS()
class CORE_SYSTEM_API UCoreReplicationGraphNode_PlayerStateFrequencyLimiter : public UReplicationGraphNode
{
	GENERATED_BODY()

	UCoreReplicationGraphNode_PlayerStateFrequencyLimiter(FVTableHelper& VTableHelper);

	virtual void NotifyAddNetworkActor(const FNewReplicatedActorInfo& Actor) override { }
	virtual bool NotifyRemoveNetworkActor(const FNewReplicatedActorInfo& ActorInfo, bool bWarnIfNotFound=true) override { return false; }
	virtual bool NotifyActorRenamed(const FRenamedReplicatedActorInfo& Actor, bool bWarnIfNotFound=true) override { return false; }

	virtual void GatherActorListsForConnection(const FConnectionGatherActorListParameters& Params) override;

	virtual void PrepareForReplication() override;

	virtual void LogNode(FReplicationGraphDebugInfo& DebugInfo, const FString& NodeName) const override;

	/** How many actors we want to return to the replication driver per frame. Will not suppress ForceNetUpdate. */
	int32 TargetActorsPerFrame = 2;

private:
	
	TArray<FActorRepListRefView> ReplicationActorLists;
	FActorRepListRefView ForceNetUpdateReplicationActorList;
};
