#include "Core_Structure.h"

DEFINE_LOG_CATEGORY(Core_Structure);

#define LOCTEXT_NAMESPACE "FCore_Structure"

void FCore_Structure::StartupModule()
{
	UE_LOG(Core_Structure, Log, TEXT("Core_Structure module has been loaded"));
}

void FCore_Structure::ShutdownModule()
{
	UE_LOG(Core_Structure, Log, TEXT("Core_Structure module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCore_Structure, Core_Structure)
