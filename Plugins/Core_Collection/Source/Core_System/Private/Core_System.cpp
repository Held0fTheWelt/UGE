#include "Core_System.h"
#include "Replication/CoreReplicationGraph.h"
#include "Interfaces/DevelopmentStaticsProviderInterface.h"
#include "DevelopmentStaticsProvider_Core.h"

DEFINE_LOG_CATEGORY(Core_System);

#define LOCTEXT_NAMESPACE "FCore_System"

void CoreReplicationGraphForceLink()
{
	CoreReplicationGraphForceLinkDummy(); // Pull in CoreReplicationGraph.cpp.obj (defines UCoreReplicationGraphNode_PlayerStateFrequencyLimiter ctor)
}

void FCore_System::StartupModule()
{
	CoreReplicationGraphForceLink();
	DevelopmentStaticsProviderRegistry::Set(Core_SystemDevelopmentStatics::GetProvider());
	UE_LOG(Core_System, Display, TEXT("Core_System module has been loaded"));
}

void FCore_System::ShutdownModule()
{
	DevelopmentStaticsProviderRegistry::Set(nullptr);
	UE_LOG(Core_System, Display, TEXT("Core_System module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCore_System, Core_System)