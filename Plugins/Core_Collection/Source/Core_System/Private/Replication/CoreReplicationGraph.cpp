// Copyright Epic Games, Inc. All Rights Reserved.
// Core_Collection

/**
*	===================== CoreReplicationGraph Replication =====================
*	Overview: See CoreReplicationGraph.h. Core nodes: UCoreReplicationGraphNode_AlwaysRelevant_ForConnection,
*	UCoreReplicationGraphNode_PlayerStateFrequencyLimiter. Debug: Core.RepGraph.PrintRouting
*/

#include "Replication/CoreReplicationGraph.h"

#include "Net/UnrealNetwork.h"
#include "Engine/LevelStreaming.h"
#include "EngineUtils.h"
#include "CoreGlobals.h"

#if WITH_GAMEPLAY_DEBUGGER
#include "GameplayDebuggerCategoryReplicator.h"
#endif

#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/Pawn.h"
#include "Engine/LevelScriptActor.h"
#include "Engine/NetConnection.h"
#include "UObject/UObjectIterator.h"

#include "Replication/CoreReplicationGraphSettings.h"
#include "Interfaces/ReplicationGraphCharacterInterface.h"
#include "Interfaces/ReplicationGraphCharacterProviderInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameStateBase.h"

DEFINE_LOG_CATEGORY( LogCoreRepGraph );

namespace Core::RepGraph
{
	float DestructionInfoMaxDist = 30000.f;
	static FAutoConsoleVariableRef CVarCoreRepGraphDestructMaxDist(TEXT("Core.RepGraph.DestructInfo.MaxDist"), DestructionInfoMaxDist, TEXT("Max distance (not squared) to rep destruct infos at"), ECVF_Default);

	int32 DisplayClientLevelStreaming = 0;
	static FAutoConsoleVariableRef CVarCoreRepGraphDisplayClientLevelStreaming(TEXT("Core.RepGraph.DisplayClientLevelStreaming"), DisplayClientLevelStreaming, TEXT(""), ECVF_Default);

	float CellSize = 10000.f;
	static FAutoConsoleVariableRef CVarCoreRepGraphCellSize(TEXT("Core.RepGraph.CellSize"), CellSize, TEXT(""), ECVF_Default);

	float SpatialBiasX = -150000.f;
	static FAutoConsoleVariableRef CVarCoreRepGraphSpatialBiasX(TEXT("Core.RepGraph.SpatialBiasX"), SpatialBiasX, TEXT(""), ECVF_Default);

	float SpatialBiasY = -200000.f;
	static FAutoConsoleVariableRef CVarCoreRepSpatialBiasY(TEXT("Core.RepGraph.SpatialBiasY"), SpatialBiasY, TEXT(""), ECVF_Default);

	int32 DynamicActorFrequencyBuckets = 3;
	static FAutoConsoleVariableRef CVarCoreRepDynamicActorFrequencyBuckets(TEXT("Core.RepGraph.DynamicActorFrequencyBuckets"), DynamicActorFrequencyBuckets, TEXT(""), ECVF_Default);

	int32 DisableSpatialRebuilds = 1;
	static FAutoConsoleVariableRef CVarCoreRepDisableSpatialRebuilds(TEXT("Core.RepGraph.DisableSpatialRebuilds"), DisableSpatialRebuilds, TEXT(""), ECVF_Default);

	int32 LogLazyInitClasses = 0;
	static FAutoConsoleVariableRef CVarCoreRepLogLazyInitClasses(TEXT("Core.RepGraph.LogLazyInitClasses"), LogLazyInitClasses, TEXT(""), ECVF_Default);

	int32 TargetKBytesSecFastSharedPath = 10;
	static FAutoConsoleVariableRef CVarCoreRepTargetKBytesSecFastSharedPath(TEXT("Core.RepGraph.TargetKBytesSecFastSharedPath"), TargetKBytesSecFastSharedPath, TEXT(""), ECVF_Default);

	float FastSharedPathCullDistPct = 0.80f;
	static FAutoConsoleVariableRef CVarCoreRepFastSharedPathCullDistPct(TEXT("Core.RepGraph.FastSharedPathCullDistPct"), FastSharedPathCullDistPct, TEXT(""), ECVF_Default);

	int32 EnableFastSharedPath = 1;
	static FAutoConsoleVariableRef CVarCoreRepEnableFastSharedPath(TEXT("Core.RepGraph.EnableFastSharedPath"), EnableFastSharedPath, TEXT(""), ECVF_Default);

	UReplicationDriver* ConditionalCreateReplicationDriver(UNetDriver* ForNetDriver, UWorld* World)
	{
		if (World && ForNetDriver && ForNetDriver->NetDriverName == NAME_GameNetDriver)
		{
			const UCoreReplicationGraphSettings* CoreRepGraphSettings = GetDefault<UCoreReplicationGraphSettings>();

			if (CoreRepGraphSettings && CoreRepGraphSettings->bDisableReplicationGraph)
			{
				UE_LOG(LogCoreRepGraph, Display, TEXT("Replication graph is disabled via CoreReplicationGraphSettings."));
				return nullptr;
			}

			UE_LOG(LogCoreRepGraph, Display, TEXT("Replication graph is enabled for %s in world %s."), *GetNameSafe(ForNetDriver), *GetPathNameSafe(World));

			TSubclassOf<UCoreReplicationGraph> GraphClass = CoreRepGraphSettings->DefaultReplicationGraphClass.TryLoadClass<UCoreReplicationGraph>();
			if (GraphClass.Get() == nullptr)
			{
				GraphClass = UCoreReplicationGraph::StaticClass();
			}

			UCoreReplicationGraph* CoreReplicationGraph = NewObject<UCoreReplicationGraph>(GetTransientPackage(), GraphClass.Get());
			return CoreReplicationGraph;
		}

		return nullptr;
	}
};

// ----------------------------------------------------------------------------------------------------------

UCoreReplicationGraph::UCoreReplicationGraph()
{
	if (!UReplicationDriver::CreateReplicationDriverDelegate().IsBound())
	{
		UReplicationDriver::CreateReplicationDriverDelegate().BindLambda(
			[](UNetDriver* ForNetDriver, const FURL& URL, UWorld* World) -> UReplicationDriver*
			{
				return Core::RepGraph::ConditionalCreateReplicationDriver(ForNetDriver, World);
			});
	}
}

void UCoreReplicationGraph::ResetGameWorldState()
{
	Super::ResetGameWorldState();

	AlwaysRelevantStreamingLevelActors.Empty();

	for (UNetReplicationGraphConnection* ConnManager : Connections)
	{
		for (UReplicationGraphNode* ConnectionNode : ConnManager->GetConnectionGraphNodes())
		{
			if (UCoreReplicationGraphNode_AlwaysRelevant_ForConnection* AlwaysRelevantConnectionNode = Cast<UCoreReplicationGraphNode_AlwaysRelevant_ForConnection>(ConnectionNode))
			{
				AlwaysRelevantConnectionNode->ResetGameWorldState();
			}
		}
	}

	for (UNetReplicationGraphConnection* ConnManager : PendingConnections)
	{
		for (UReplicationGraphNode* ConnectionNode : ConnManager->GetConnectionGraphNodes())
		{
			if (UCoreReplicationGraphNode_AlwaysRelevant_ForConnection* AlwaysRelevantConnectionNode = Cast<UCoreReplicationGraphNode_AlwaysRelevant_ForConnection>(ConnectionNode))
			{
				AlwaysRelevantConnectionNode->ResetGameWorldState();
			}
		}
	}
}

ECoreClassRepNodeMapping UCoreReplicationGraph::GetClassNodeMapping(UClass* Class) const
{
	if (!Class)
	{
		return ECoreClassRepNodeMapping::NotRouted;
	}
	
	if (const ECoreClassRepNodeMapping* Ptr = ClassRepNodePolicies.FindWithoutClassRecursion(Class))
	{
		return *Ptr;
	}
	
	AActor* ActorCDO = Cast<AActor>(Class->GetDefaultObject());
	if (!ActorCDO || !ActorCDO->GetIsReplicated())
	{
		return ECoreClassRepNodeMapping::NotRouted;
	}
		
	auto ShouldSpatialize = [](const AActor* CDO)
	{
		return CDO->GetIsReplicated() && (!(CDO->bAlwaysRelevant || CDO->bOnlyRelevantToOwner || CDO->bNetUseOwnerRelevancy));
	};

	auto GetLegacyDebugStr = [](const AActor* CDO)
	{
		return FString::Printf(TEXT("%s [%d/%d/%d]"), *CDO->GetClass()->GetName(), CDO->bAlwaysRelevant, CDO->bOnlyRelevantToOwner, CDO->bNetUseOwnerRelevancy);
	};

	// Only handle this class if it differs from its super. There is no need to put every child class explicitly in the graph class mapping
	UClass* SuperClass = Class->GetSuperClass();
	if (AActor* SuperCDO = Cast<AActor>(SuperClass->GetDefaultObject()))
	{
		if (SuperCDO->GetIsReplicated() == ActorCDO->GetIsReplicated()
			&& SuperCDO->bAlwaysRelevant == ActorCDO->bAlwaysRelevant
			&& SuperCDO->bOnlyRelevantToOwner == ActorCDO->bOnlyRelevantToOwner
			&& SuperCDO->bNetUseOwnerRelevancy == ActorCDO->bNetUseOwnerRelevancy
			)
		{
			return GetClassNodeMapping(SuperClass);
		}
	}

	if (ShouldSpatialize(ActorCDO))
	{
		return ECoreClassRepNodeMapping::Spatialize_Dynamic;
	}
	else if (ActorCDO->bAlwaysRelevant && !ActorCDO->bOnlyRelevantToOwner)
	{
		return ECoreClassRepNodeMapping::RelevantAllConnections;
	}

	return ECoreClassRepNodeMapping::NotRouted;
}

void UCoreReplicationGraph::RegisterClassRepNodeMapping(UClass* Class)
{
	ECoreClassRepNodeMapping Mapping = GetClassNodeMapping(Class);
	ClassRepNodePolicies.Set(Class, Mapping);
}

void UCoreReplicationGraph::InitClassReplicationInfo(FClassReplicationInfo& Info, UClass* Class, bool Spatialize) const
{
	AActor* CDO = Class->GetDefaultObject<AActor>();
	if (Spatialize)
	{
		Info.SetCullDistanceSquared(CDO->GetNetCullDistanceSquared());
		UE_LOG(LogCoreRepGraph, Log, TEXT("Setting cull distance for %s to %f (%f)"), *Class->GetName(), Info.GetCullDistanceSquared(), Info.GetCullDistance());
	}

	Info.ReplicationPeriodFrame = GetReplicationPeriodFrameForFrequency(CDO->GetNetUpdateFrequency());

	UClass* NativeClass = Class;
	while (!NativeClass->IsNative() && NativeClass->GetSuperClass() && NativeClass->GetSuperClass() != AActor::StaticClass())
	{
		NativeClass = NativeClass->GetSuperClass();
	}

	UE_LOG(LogCoreRepGraph, Log, TEXT("Setting replication period for %s (%s) to %d frames (%.2f)"), *Class->GetName(), *NativeClass->GetName(), Info.ReplicationPeriodFrame, CDO->GetNetUpdateFrequency());
}

bool UCoreReplicationGraph::ConditionalInitClassReplicationInfo(UClass* ReplicatedClass, FClassReplicationInfo& ClassInfo)
{
	if (ExplicitlySetClasses.FindByPredicate([&](const UClass* SetClass) { return ReplicatedClass->IsChildOf(SetClass); }) != nullptr)
	{
		return false;
	}

	bool ClassIsSpatialized = IsSpatialized(ClassRepNodePolicies.GetChecked(ReplicatedClass));
	InitClassReplicationInfo(ClassInfo, ReplicatedClass, ClassIsSpatialized);
	return true;
}

void UCoreReplicationGraph::AddClassRepInfo(UClass* Class, ECoreClassRepNodeMapping Mapping)
{
	if (IsSpatialized(Mapping))
	{
		if (Class->GetDefaultObject<AActor>()->bAlwaysRelevant)
		{
			UE_LOG(LogCoreRepGraph, Warning, TEXT("Replicated Class %s is AlwaysRelevant but is initialized into a spatialized node (%s)"), *Class->GetName(), *StaticEnum<ECoreClassRepNodeMapping>()->GetNameStringByValue((int64)Mapping));
		}
	}

	ClassRepNodePolicies.Set(Class, Mapping);
}

void UCoreReplicationGraph::RegisterClassReplicationInfo(UClass* ReplicatedClass)
{
	FClassReplicationInfo ClassInfo;
	if (ConditionalInitClassReplicationInfo(ReplicatedClass, ClassInfo))
	{
		GlobalActorReplicationInfoMap.SetClassInfo(ReplicatedClass, ClassInfo);
		UE_LOG(LogCoreRepGraph, Log, TEXT("Setting %s - %.2f"), *GetNameSafe(ReplicatedClass), ClassInfo.GetCullDistance());
	}
}

void UCoreReplicationGraph::InitGlobalActorClassSettings()
{
	// Setup our lazy init function for classes that are not currently loaded.
	GlobalActorReplicationInfoMap.SetInitClassInfoFunc(
		[this](UClass* Class, FClassReplicationInfo& ClassInfo)
		{
			RegisterClassRepNodeMapping(Class); // This needs to run before RegisterClassReplicationInfo.

			const bool bHandled = ConditionalInitClassReplicationInfo(Class, ClassInfo);

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
			if (Core::RepGraph::LogLazyInitClasses != 0)
			{
				if (bHandled)
				{
					ECoreClassRepNodeMapping Mapping = ClassRepNodePolicies.GetChecked(Class);
					UE_LOG(LogCoreRepGraph, Warning, TEXT("%s was Lazy Initialized. (Parent: %s) %d."), *GetNameSafe(Class), *GetNameSafe(Class->GetSuperClass()), (int32)Mapping);

					FClassReplicationInfo& ParentRepInfo = GlobalActorReplicationInfoMap.GetClassInfo(Class->GetSuperClass());
					if (ClassInfo.BuildDebugStringDelta() != ParentRepInfo.BuildDebugStringDelta())
					{
						UE_LOG(LogCoreRepGraph, Warning, TEXT("Differences Found!"));
						FString DebugStr = ParentRepInfo.BuildDebugStringDelta();
						UE_LOG(LogCoreRepGraph, Warning, TEXT("  Parent: %s"), *DebugStr);

						DebugStr = ClassInfo.BuildDebugStringDelta();
						UE_LOG(LogCoreRepGraph, Warning, TEXT("  Class : %s"), *DebugStr);
					}
				}
				else
				{
					UE_LOG(LogCoreRepGraph, Warning, TEXT("%s skipped Lazy Initialization because it does not differ from its parent. (Parent: %s)"), *GetNameSafe(Class), *GetNameSafe(Class->GetSuperClass()));

				}
			}
#endif

			return bHandled;
		});

	ClassRepNodePolicies.InitNewElement = [this](UClass* Class, ECoreClassRepNodeMapping& NodeMapping)
	{
		NodeMapping = GetClassNodeMapping(Class);
		return true;
	};

	const UCoreReplicationGraphSettings* CoreRepGraphSettings = GetDefault<UCoreReplicationGraphSettings>();
	check(CoreRepGraphSettings);

	// Set Classes Node Mappings
	for (const FCoreRepGraphActorClassSettings& ActorClassSettings : CoreRepGraphSettings->ClassSettings)
	{
		if (ActorClassSettings.bAddClassRepInfoToMap)
		{
			if (UClass* StaticActorClass = ActorClassSettings.GetStaticActorClass())
			{
				UE_LOG(LogCoreRepGraph, Log, TEXT("ActorClassSettings -- AddClassRepInfo - %s :: %i"), *StaticActorClass->GetName(), int(ActorClassSettings.ClassNodeMapping));
				AddClassRepInfo(StaticActorClass, ActorClassSettings.ClassNodeMapping);
			}
		}
	}

#if WITH_GAMEPLAY_DEBUGGER
	AddClassRepInfo(AGameplayDebuggerCategoryReplicator::StaticClass(), ECoreClassRepNodeMapping::NotRouted);				// Replicated via UCoreReplicationGraphNode_AlwaysRelevant_ForConnection
#endif

	TArray<UClass*> AllReplicatedClasses;

	for (TObjectIterator<UClass> It; It; ++It)
	{
		UClass* Class = *It;
		AActor* ActorCDO = Cast<AActor>(Class->GetDefaultObject());
		if (!ActorCDO || !ActorCDO->GetIsReplicated())
		{
			continue;
		}

		// Skip SKEL and REINST classes. I don't know a better way to do this.
		if (Class->GetName().StartsWith(TEXT("SKEL_")) || Class->GetName().StartsWith(TEXT("REINST_")))
		{
			continue;
		}

		// --------------------------------------------------------------------
		// This is a replicated class. Save this off for the second pass below
		// --------------------------------------------------------------------

		AllReplicatedClasses.Add(Class);

		RegisterClassRepNodeMapping(Class);
	}

	// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Setup FClassReplicationInfo. This is essentially the per class replication settings. Some we set explicitly, the rest we are setting via looking at the legacy settings on AActor.
	// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	auto SetClassInfo = [&](UClass* Class, const FClassReplicationInfo& Info) { GlobalActorReplicationInfoMap.SetClassInfo(Class, Info); ExplicitlySetClasses.Add(Class); };
	ExplicitlySetClasses.Reset();

	// Get replication character class from game state (e.g. ACoreCharacter) via interface to avoid Core_System depending on Core_Classes
	UClass* ReplicationCharacterClass = ACharacter::StaticClass();
	if (UWorld* World = GetWorld())
	{
		if (AGameStateBase* GameState = World->GetGameState())
		{
			IReplicationGraphCharacterProviderInterface* Provider = Cast<IReplicationGraphCharacterProviderInterface>(GameState);
			if (Provider)
			{
				if (UClass* ProviderClass = Provider->GetReplicationGraphCharacterClass())
				{
					ReplicationCharacterClass = ProviderClass;
				}
			}
		}
	}

	FClassReplicationInfo CharacterClassRepInfo;
	CharacterClassRepInfo.DistancePriorityScale = 1.f;
	CharacterClassRepInfo.StarvationPriorityScale = 1.f;
	CharacterClassRepInfo.ActorChannelFrameTimeout = 4;
	if (ACharacter* CharCDO = Cast<ACharacter>(ReplicationCharacterClass->GetDefaultObject()))
	{
		CharacterClassRepInfo.SetCullDistanceSquared(CharCDO->GetNetCullDistanceSquared());
	}
	SetClassInfo(ACharacter::StaticClass(), CharacterClassRepInfo);

	// ------------------------------------------------------------------------------------------------------
	//	Setup FastShared replication for pawns. This is called up to once per frame per pawn to see if it wants
	//	to send a FastShared update to all relevant connections.
	// ------------------------------------------------------------------------------------------------------
	CharacterClassRepInfo.FastSharedReplicationFunc = [](AActor* Actor)
	{
		bool bSuccess = false;
		IReplicationGraphCharacterInterface* CharInterface = Cast<IReplicationGraphCharacterInterface>(Actor);
		if (CharInterface)
		{
			bSuccess = CharInterface->UpdateSharedReplication();
		}
		return bSuccess;
	};

	CharacterClassRepInfo.FastSharedReplicationFuncName = FName(TEXT("FastSharedReplication"));

	FastSharedPathConstants.MaxBitsPerFrame = (int32)((float)(Core::RepGraph::TargetKBytesSecFastSharedPath * 1024 * 8) / NetDriver->GetNetServerMaxTickRate());
	FastSharedPathConstants.DistanceRequirementPct = Core::RepGraph::FastSharedPathCullDistPct;

	SetClassInfo(ReplicationCharacterClass, CharacterClassRepInfo);

	// ---------------------------------------------------------------------
	UReplicationGraphNode_ActorListFrequencyBuckets::DefaultSettings.ListSize = 12;
	UReplicationGraphNode_ActorListFrequencyBuckets::DefaultSettings.NumBuckets = Core::RepGraph::DynamicActorFrequencyBuckets;
	UReplicationGraphNode_ActorListFrequencyBuckets::DefaultSettings.BucketThresholds.Reset();
	UReplicationGraphNode_ActorListFrequencyBuckets::DefaultSettings.EnableFastPath = (Core::RepGraph::EnableFastSharedPath > 0);
	UReplicationGraphNode_ActorListFrequencyBuckets::DefaultSettings.FastPathFrameModulo = 1;

	RPCSendPolicyMap.Reset();

	// Set FClassReplicationInfo based on legacy settings from all replicated classes
	for (UClass* ReplicatedClass : AllReplicatedClasses)
	{
		RegisterClassReplicationInfo(ReplicatedClass);
	}

	// Print out what we came up with
	UE_LOG(LogCoreRepGraph, Log, TEXT(""));
	UE_LOG(LogCoreRepGraph, Log, TEXT("Class Routing Map: "));
	for (auto ClassMapIt = ClassRepNodePolicies.CreateIterator(); ClassMapIt; ++ClassMapIt)
	{
		UClass* Class = CastChecked<UClass>(ClassMapIt.Key().ResolveObjectPtr());
		ECoreClassRepNodeMapping Mapping = ClassMapIt.Value();

		// Only print if different than native class
		UClass* ParentNativeClass = GetParentNativeClass(Class);

		ECoreClassRepNodeMapping* ParentMapping = ClassRepNodePolicies.Get(ParentNativeClass);
		if (ParentMapping && Class != ParentNativeClass && Mapping == *ParentMapping)
		{
			continue;
		}

		UE_LOG(LogCoreRepGraph, Log, TEXT("  %s (%s) -> %s"), *Class->GetName(), *GetNameSafe(ParentNativeClass), *StaticEnum<ECoreClassRepNodeMapping>()->GetNameStringByValue((int64)Mapping));
	}

	UE_LOG(LogCoreRepGraph, Log, TEXT(""));
	UE_LOG(LogCoreRepGraph, Log, TEXT("Class Settings Map: "));
	FClassReplicationInfo DefaultValues;
	for (auto ClassRepInfoIt = GlobalActorReplicationInfoMap.CreateClassMapIterator(); ClassRepInfoIt; ++ClassRepInfoIt)
	{
		UClass* Class = CastChecked<UClass>(ClassRepInfoIt.Key().ResolveObjectPtr());
		const FClassReplicationInfo& ClassInfo = ClassRepInfoIt.Value();
		UE_LOG(LogCoreRepGraph, Log, TEXT("  %s (%s) -> %s"), *Class->GetName(), *GetNameSafe(GetParentNativeClass(Class)), *ClassInfo.BuildDebugStringDelta());
	}


	// Rep destruct infos based on CVar value
	DestructInfoMaxDistanceSquared = Core::RepGraph::DestructionInfoMaxDist * Core::RepGraph::DestructionInfoMaxDist;

#if WITH_GAMEPLAY_DEBUGGER
	AGameplayDebuggerCategoryReplicator::NotifyDebuggerOwnerChange.AddUObject(this, &ThisClass::OnGameplayDebuggerOwnerChange);
#endif

	// Add to RPC_Multicast_OpenChannelForClass map
	RPC_Multicast_OpenChannelForClass.Reset();
	RPC_Multicast_OpenChannelForClass.Set(AActor::StaticClass(), true); // Open channels for multicast RPCs by default
	RPC_Multicast_OpenChannelForClass.Set(AController::StaticClass(), false); // multicasts should never open channels on Controllers since opening a channel on a non-owner breaks the Controller's replication.
	RPC_Multicast_OpenChannelForClass.Set(AServerStatReplicator::StaticClass(), false);

	for (const FCoreRepGraphActorClassSettings& ActorClassSettings : CoreRepGraphSettings->ClassSettings)
	{
		if (ActorClassSettings.bAddToRPC_Multicast_OpenChannelForClassMap)
		{
			if (UClass* StaticActorClass = ActorClassSettings.GetStaticActorClass())
			{
				UE_LOG(LogCoreRepGraph, Log, TEXT("ActorClassSettings -- RPC_Multicast_OpenChannelForClass - %s"), *StaticActorClass->GetName());
				RPC_Multicast_OpenChannelForClass.Set(StaticActorClass, ActorClassSettings.bRPC_Multicast_OpenChannelForClass);
			}
		}
	}
}

void UCoreReplicationGraph::InitGlobalGraphNodes()
{
	// -----------------------------------------------
	//	Spatial Actors
	// -----------------------------------------------

	GridNode = CreateNewNode<UReplicationGraphNode_GridSpatialization2D>();
	GridNode->CellSize = Core::RepGraph::CellSize;
	GridNode->SpatialBias = FVector2D(Core::RepGraph::SpatialBiasX, Core::RepGraph::SpatialBiasY);

	if (Core::RepGraph::DisableSpatialRebuilds)
	{
		GridNode->AddToClassRebuildDenyList(AActor::StaticClass()); // Disable All spatial rebuilding
	}
	
	AddGlobalGraphNode(GridNode);

	// -----------------------------------------------
	//	Always Relevant (to everyone) Actors
	// -----------------------------------------------
	AlwaysRelevantNode = CreateNewNode<UReplicationGraphNode_ActorList>();
	AddGlobalGraphNode(AlwaysRelevantNode);

	// -----------------------------------------------
	//	Player State specialization. This will return a rolling subset of the player states to replicate
	// -----------------------------------------------
	UCoreReplicationGraphNode_PlayerStateFrequencyLimiter* PlayerStateNode = CreateNewNode<UCoreReplicationGraphNode_PlayerStateFrequencyLimiter>();
	AddGlobalGraphNode(PlayerStateNode);
}

void UCoreReplicationGraph::InitConnectionGraphNodes(UNetReplicationGraphConnection* RepGraphConnection)
{
	Super::InitConnectionGraphNodes(RepGraphConnection);

	UCoreReplicationGraphNode_AlwaysRelevant_ForConnection* AlwaysRelevantConnectionNode = CreateNewNode<UCoreReplicationGraphNode_AlwaysRelevant_ForConnection>();

	// This node needs to know when client levels go in and out of visibility
	RepGraphConnection->OnClientVisibleLevelNameAdd.AddUObject(AlwaysRelevantConnectionNode, &UCoreReplicationGraphNode_AlwaysRelevant_ForConnection::OnClientLevelVisibilityAdd);
	RepGraphConnection->OnClientVisibleLevelNameRemove.AddUObject(AlwaysRelevantConnectionNode, &UCoreReplicationGraphNode_AlwaysRelevant_ForConnection::OnClientLevelVisibilityRemove);

	AddConnectionGraphNode(AlwaysRelevantConnectionNode, RepGraphConnection);
}

ECoreClassRepNodeMapping UCoreReplicationGraph::GetMappingPolicy(UClass* Class)
{
	ECoreClassRepNodeMapping* PolicyPtr = ClassRepNodePolicies.Get(Class);
	ECoreClassRepNodeMapping Policy = PolicyPtr ? *PolicyPtr : ECoreClassRepNodeMapping::NotRouted;
	return Policy;
}

void UCoreReplicationGraph::RouteAddNetworkActorToNodes(const FNewReplicatedActorInfo& ActorInfo, FGlobalActorReplicationInfo& GlobalInfo)
{
	ECoreClassRepNodeMapping Policy = GetMappingPolicy(ActorInfo.Class);
	switch(Policy)
	{
		case ECoreClassRepNodeMapping::NotRouted:
		{
			break;
		}
		
		case ECoreClassRepNodeMapping::RelevantAllConnections:
		{
			if (ActorInfo.StreamingLevelName == NAME_None)
			{
				AlwaysRelevantNode->NotifyAddNetworkActor(ActorInfo);
			}
			else
			{
				FActorRepListRefView& RepList = AlwaysRelevantStreamingLevelActors.FindOrAdd(ActorInfo.StreamingLevelName);
				RepList.ConditionalAdd(ActorInfo.Actor);
			}
			break;
		}

		case ECoreClassRepNodeMapping::Spatialize_Static:
		{
			GridNode->AddActor_Static(ActorInfo, GlobalInfo);
			break;
		}
		
		case ECoreClassRepNodeMapping::Spatialize_Dynamic:
		{
			GridNode->AddActor_Dynamic(ActorInfo, GlobalInfo);
			break;
		}
		
		case ECoreClassRepNodeMapping::Spatialize_Dormancy:
		{
			GridNode->AddActor_Dormancy(ActorInfo, GlobalInfo);
			break;
		}
	};
}

void UCoreReplicationGraph::RouteRemoveNetworkActorToNodes(const FNewReplicatedActorInfo& ActorInfo)
{
	ECoreClassRepNodeMapping Policy = GetMappingPolicy(ActorInfo.Class);
	switch(Policy)
	{
		case ECoreClassRepNodeMapping::NotRouted:
		{
			break;
		}
		
		case ECoreClassRepNodeMapping::RelevantAllConnections:
		{
			if (ActorInfo.StreamingLevelName == NAME_None)
			{
				AlwaysRelevantNode->NotifyRemoveNetworkActor(ActorInfo);
			}
			else
			{
				FActorRepListRefView& RepList = AlwaysRelevantStreamingLevelActors.FindChecked(ActorInfo.StreamingLevelName);
				if (RepList.RemoveFast(ActorInfo.Actor) == false)
				{
					UE_LOG(LogCoreRepGraph, Warning, TEXT("Actor %s was not found in AlwaysRelevantStreamingLevelActors list. LevelName: %s"), *GetActorRepListTypeDebugString(ActorInfo.Actor), *ActorInfo.StreamingLevelName.ToString());
				}				
			}

			SetActorDestructionInfoToIgnoreDistanceCulling(ActorInfo.GetActor());

			break;
		}

		case ECoreClassRepNodeMapping::Spatialize_Static:
		{
			GridNode->RemoveActor_Static(ActorInfo);
			break;
		}
		
		case ECoreClassRepNodeMapping::Spatialize_Dynamic:
		{
			GridNode->RemoveActor_Dynamic(ActorInfo);
			break;
		}
		
		case ECoreClassRepNodeMapping::Spatialize_Dormancy:
		{
			GridNode->RemoveActor_Dormancy(ActorInfo);
			break;
		}
	};
}

// Since we listen to global (static) events, we need to watch out for cross world broadcasts (PIE)
#if WITH_EDITOR
#define CHECK_WORLDS(X) if(X->GetWorld() != GetWorld()) return;
#else
#define CHECK_WORLDS(X)
#endif

#if WITH_GAMEPLAY_DEBUGGER
void UCoreReplicationGraph::OnGameplayDebuggerOwnerChange(AGameplayDebuggerCategoryReplicator* Debugger, APlayerController* OldOwner)
{
	CHECK_WORLDS(Debugger);

	auto GetAlwaysRelevantForConnectionNode = [this](APlayerController* Controller) -> UCoreReplicationGraphNode_AlwaysRelevant_ForConnection*
	{
		if (Controller)
		{
			if (UNetConnection* NetConnection = Controller->GetNetConnection())
			{
				if (NetConnection->GetDriver() == NetDriver)
				{
					if (UNetReplicationGraphConnection* GraphConnection = FindOrAddConnectionManager(NetConnection))
					{
						for (UReplicationGraphNode* ConnectionNode : GraphConnection->GetConnectionGraphNodes())
						{
							if (UCoreReplicationGraphNode_AlwaysRelevant_ForConnection* AlwaysRelevantConnectionNode = Cast<UCoreReplicationGraphNode_AlwaysRelevant_ForConnection>(ConnectionNode))
							{
								return AlwaysRelevantConnectionNode;
							}
						}
					}
				}
			}
		}

		return nullptr;
	};

	if (UCoreReplicationGraphNode_AlwaysRelevant_ForConnection* AlwaysRelevantConnectionNode = GetAlwaysRelevantForConnectionNode(OldOwner))
	{
		AlwaysRelevantConnectionNode->GameplayDebugger = nullptr;
	}

	if (UCoreReplicationGraphNode_AlwaysRelevant_ForConnection* AlwaysRelevantConnectionNode = GetAlwaysRelevantForConnectionNode(Debugger->GetReplicationOwner()))
	{
		AlwaysRelevantConnectionNode->GameplayDebugger = Debugger;
	}
}
#endif

#undef CHECK_WORLDS

// ------------------------------------------------------------------------------

void UCoreReplicationGraphNode_AlwaysRelevant_ForConnection::ResetGameWorldState()
{
	ReplicationActorList.Reset();
	AlwaysRelevantStreamingLevelsNeedingReplication.Empty();
}

void UCoreReplicationGraphNode_AlwaysRelevant_ForConnection::GatherActorListsForConnection(const FConnectionGatherActorListParameters& Params)
{
	UCoreReplicationGraph* CoreGraph = CastChecked<UCoreReplicationGraph>(GetOuter());

	ReplicationActorList.Reset();

	for (const FNetViewer& CurViewer : Params.Viewers)
	{
		ReplicationActorList.ConditionalAdd(CurViewer.InViewer);
		ReplicationActorList.ConditionalAdd(CurViewer.ViewTarget);

		if (APlayerController* PC = Cast<APlayerController>(CurViewer.InViewer))
		{
			// 50% throttling of PlayerStates.
			const bool bReplicatePS = (Params.ConnectionManager.ConnectionOrderNum % 2) == (Params.ReplicationFrameNum % 2);
			if (bReplicatePS)
			{
				// Always return the player state to the owning player. Simulated proxy player states are handled by UCoreReplicationGraphNode_PlayerStateFrequencyLimiter
				if (APlayerState* PS = PC->PlayerState)
				{
					if (!bInitializedPlayerState)
					{
						bInitializedPlayerState = true;
						FConnectionReplicationActorInfo& ConnectionActorInfo = Params.ConnectionManager.ActorInfoMap.FindOrAdd(PS);
						ConnectionActorInfo.ReplicationPeriodFrame = 1;
					}

					ReplicationActorList.ConditionalAdd(PS);
				}
			}

			FCachedAlwaysRelevantActorInfo& LastData = PastRelevantActorMap.FindOrAdd(CurViewer.Connection);

			if (AActor* Pawn = PC->GetPawn())
			{
				if (Pawn->GetClass()->ImplementsInterface(UReplicationGraphCharacterInterface::StaticClass()))
				{
					UpdateCachedRelevantActor(Params, Pawn, LastData.LastViewer);

					if (Pawn != CurViewer.ViewTarget)
					{
						ReplicationActorList.ConditionalAdd(Pawn);
					}
				}
			}

			if (AActor* ViewTargetPawn = CurViewer.ViewTarget)
			{
				if (ViewTargetPawn->GetClass()->ImplementsInterface(UReplicationGraphCharacterInterface::StaticClass()))
				{
					UpdateCachedRelevantActor(Params, ViewTargetPawn, LastData.LastViewTarget);
				}
			}
		}
	}

	CleanupCachedRelevantActors(PastRelevantActorMap);

	// Always relevant streaming level actors.
	FPerConnectionActorInfoMap& ConnectionActorInfoMap = Params.ConnectionManager.ActorInfoMap;
	
	TMap<FName, FActorRepListRefView>& AlwaysRelevantStreamingLevelActors = CoreGraph->AlwaysRelevantStreamingLevelActors;

	for (int32 Idx=AlwaysRelevantStreamingLevelsNeedingReplication.Num()-1; Idx >= 0; --Idx)
	{
		const FName& StreamingLevel = AlwaysRelevantStreamingLevelsNeedingReplication[Idx];

		FActorRepListRefView* Ptr = AlwaysRelevantStreamingLevelActors.Find(StreamingLevel);
		if (Ptr == nullptr)
		{
			// No always relevant lists for that level
			UE_CLOG(Core::RepGraph::DisplayClientLevelStreaming > 0, LogCoreRepGraph, Display, TEXT("CLIENTSTREAMING Removing %s from AlwaysRelevantStreamingLevelActors because FActorRepListRefView is null. %s "), *StreamingLevel.ToString(),  *Params.ConnectionManager.GetName());
			AlwaysRelevantStreamingLevelsNeedingReplication.RemoveAtSwap(Idx, EAllowShrinking::No);
			continue;
		}

		FActorRepListRefView& RepList = *Ptr;

		if (RepList.Num() > 0)
		{
			bool bAllDormant = true;
			for (FActorRepListType Actor : RepList)
			{
				FConnectionReplicationActorInfo& ConnectionActorInfo = ConnectionActorInfoMap.FindOrAdd(Actor);
				if (ConnectionActorInfo.bDormantOnConnection == false)
				{
					bAllDormant = false;
					break;
				}
			}

			if (bAllDormant)
			{
				UE_CLOG(Core::RepGraph::DisplayClientLevelStreaming > 0, LogCoreRepGraph, Display, TEXT("CLIENTSTREAMING All AlwaysRelevant Actors Dormant on StreamingLevel %s for %s. Removing list."), *StreamingLevel.ToString(), *Params.ConnectionManager.GetName());
				AlwaysRelevantStreamingLevelsNeedingReplication.RemoveAtSwap(Idx, EAllowShrinking::No);
			}
			else
			{
				UE_CLOG(Core::RepGraph::DisplayClientLevelStreaming > 0, LogCoreRepGraph, Display, TEXT("CLIENTSTREAMING Adding always Actors on StreamingLevel %s for %s because it has at least one non dormant actor"), *StreamingLevel.ToString(), *Params.ConnectionManager.GetName());
				Params.OutGatheredReplicationLists.AddReplicationActorList(RepList);
			}
		}
		else
		{
			UE_LOG(LogCoreRepGraph, Warning, TEXT("UCoreReplicationGraphNode_AlwaysRelevant_ForConnection::GatherActorListsForConnection - empty RepList %s"), *Params.ConnectionManager.GetName());
		}

	}

#if WITH_GAMEPLAY_DEBUGGER
	if (GameplayDebugger)
	{
		ReplicationActorList.ConditionalAdd(GameplayDebugger);
	}
#endif

	Params.OutGatheredReplicationLists.AddReplicationActorList(ReplicationActorList);
}

void UCoreReplicationGraphNode_AlwaysRelevant_ForConnection::OnClientLevelVisibilityAdd(FName LevelName, UWorld* StreamingWorld)
{
	UE_CLOG(Core::RepGraph::DisplayClientLevelStreaming > 0, LogCoreRepGraph, Display, TEXT("CLIENTSTREAMING ::OnClientLevelVisibilityAdd - %s"), *LevelName.ToString());
	AlwaysRelevantStreamingLevelsNeedingReplication.Add(LevelName);
}

void UCoreReplicationGraphNode_AlwaysRelevant_ForConnection::OnClientLevelVisibilityRemove(FName LevelName)
{
	UE_CLOG(Core::RepGraph::DisplayClientLevelStreaming > 0, LogCoreRepGraph, Display, TEXT("CLIENTSTREAMING ::OnClientLevelVisibilityRemove - %s"), *LevelName.ToString());
	AlwaysRelevantStreamingLevelsNeedingReplication.Remove(LevelName);
}

void UCoreReplicationGraphNode_AlwaysRelevant_ForConnection::LogNode(FReplicationGraphDebugInfo& DebugInfo, const FString& NodeName) const
{
	DebugInfo.Log(NodeName);
	DebugInfo.PushIndent();
	LogActorRepList(DebugInfo, NodeName, ReplicationActorList);

	for (const FName& LevelName : AlwaysRelevantStreamingLevelsNeedingReplication)
	{
		UCoreReplicationGraph* CoreGraph = CastChecked<UCoreReplicationGraph>(GetOuter());
		if (FActorRepListRefView* RepList = CoreGraph->AlwaysRelevantStreamingLevelActors.Find(LevelName))
		{
			LogActorRepList(DebugInfo, FString::Printf(TEXT("AlwaysRelevant StreamingLevel List: %s"), *LevelName.ToString()), *RepList);
		}
	}

	DebugInfo.PopIndent();
}

// ------------------------------------------------------------------------------

UCoreReplicationGraphNode_PlayerStateFrequencyLimiter::UCoreReplicationGraphNode_PlayerStateFrequencyLimiter(FVTableHelper& VTableHelper)
	: Super(VTableHelper)
{
	bRequiresPrepareForReplicationCall = true;
}

void UCoreReplicationGraphNode_PlayerStateFrequencyLimiter::PrepareForReplication()
{
	ReplicationActorLists.Reset();
	ForceNetUpdateReplicationActorList.Reset();

	ReplicationActorLists.AddDefaulted();
	FActorRepListRefView* CurrentList = &ReplicationActorLists[0];

	// We rebuild our lists of player states each frame. This is not as efficient as it could be but its the simplest way
	// to handle players disconnecting and keeping the lists compact. If the lists were persistent we would need to defrag them as players left.

	for (TActorIterator<APlayerState> It(GetWorld()); It; ++It)
	{
		APlayerState* PS = *It;
		if (IsActorValidForReplicationGather(PS) == false)
		{
			continue;
		}

		if (CurrentList->Num() >= TargetActorsPerFrame)
		{
			ReplicationActorLists.AddDefaulted();
			CurrentList = &ReplicationActorLists.Last(); 
		}
		
		CurrentList->Add(PS);
	}	
}

void UCoreReplicationGraphNode_PlayerStateFrequencyLimiter::GatherActorListsForConnection(const FConnectionGatherActorListParameters& Params)
{
	const int32 ListIdx = Params.ReplicationFrameNum % ReplicationActorLists.Num();
	Params.OutGatheredReplicationLists.AddReplicationActorList(ReplicationActorLists[ListIdx]);

	if (ForceNetUpdateReplicationActorList.Num() > 0)
	{
		Params.OutGatheredReplicationLists.AddReplicationActorList(ForceNetUpdateReplicationActorList);
	}	
}

void UCoreReplicationGraphNode_PlayerStateFrequencyLimiter::LogNode(FReplicationGraphDebugInfo& DebugInfo, const FString& NodeName) const
{
	DebugInfo.Log(NodeName);
	DebugInfo.PushIndent();	

	int32 i=0;
	for (const FActorRepListRefView& List : ReplicationActorLists)
	{
		LogActorRepList(DebugInfo, FString::Printf(TEXT("Bucket[%d]"), i++), List);
	}

	DebugInfo.PopIndent();
}

// ------------------------------------------------------------------------------

void UCoreReplicationGraph::PrintRepNodePolicies()
{
	UEnum* Enum = StaticEnum<ECoreClassRepNodeMapping>();
	if (!Enum)
	{
		return;
	}

	GLog->Logf(TEXT("===================================="));
	GLog->Logf(TEXT("Core Replication Routing Policies"));
	GLog->Logf(TEXT("===================================="));

	for (auto It = ClassRepNodePolicies.CreateIterator(); It; ++It)
	{
		FObjectKey ObjKey = It.Key();
		
		ECoreClassRepNodeMapping Mapping = It.Value();

		GLog->Logf(TEXT("%-40s --> %s"), *GetNameSafe(ObjKey.ResolveObjectPtr()), *Enum->GetNameStringByValue(static_cast<uint32>(Mapping)));
	}
}

FAutoConsoleCommandWithWorldAndArgs CorePrintRepNodePoliciesCmd(TEXT("Core.RepGraph.PrintRouting"),TEXT("Prints how actor classes are routed to RepGraph nodes"),
	FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
	{
		for (TObjectIterator<UCoreReplicationGraph> It; It; ++It)
		{
			It->PrintRepNodePolicies();
		}
	})
);

// ------------------------------------------------------------------------------

FAutoConsoleCommandWithWorldAndArgs ChangeFrequencyBucketsCmd(TEXT("Core.RepGraph.FrequencyBuckets"), TEXT("Resets frequency bucket count."), FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray< FString >& Args, UWorld* World) 
{
	int32 Buckets = 1;
	if (Args.Num() > 0)
	{
		LexTryParseString<int32>(Buckets, *Args[0]);
	}

	UE_LOG(LogCoreRepGraph, Display, TEXT("Setting Frequency Buckets to %d"), Buckets);
	for (TObjectIterator<UReplicationGraphNode_ActorListFrequencyBuckets> It; It; ++It)
	{
		UReplicationGraphNode_ActorListFrequencyBuckets* Node = *It;
		Node->SetNonStreamingCollectionSize(Buckets);
	}
}));

void CoreReplicationGraphForceLinkDummy()
{
	// Dummy symbol so Core_System.cpp's ForceLink() can reference this .cpp and the linker includes this translation unit.
}
