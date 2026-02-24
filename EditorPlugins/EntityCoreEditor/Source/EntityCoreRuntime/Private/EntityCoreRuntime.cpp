#include "EntityCoreRuntime.h"

DEFINE_LOG_CATEGORY(EntityCoreRuntime);

#define LOCTEXT_NAMESPACE "FEntityCoreRuntime"

void FEntityCoreRuntime::StartupModule()
{
	UE_LOG(EntityCoreRuntime, Warning, TEXT("EntityCoreRuntime module has been loaded"));
}

void FEntityCoreRuntime::ShutdownModule()
{
	UE_LOG(EntityCoreRuntime, Warning, TEXT("EntityCoreRuntime module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FEntityCoreRuntime, EntityCoreRuntime)