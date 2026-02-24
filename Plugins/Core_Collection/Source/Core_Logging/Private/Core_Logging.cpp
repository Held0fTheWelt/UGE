#include "Core_Logging.h"

DEFINE_LOG_CATEGORY(Core_Logging);

#define LOCTEXT_NAMESPACE "FCore_Logging"

void FCore_Logging::StartupModule()
{
	UE_LOG(Core_Logging, Log, TEXT("Core_Logging module has been loaded"));
}

void FCore_Logging::ShutdownModule()
{
	UE_LOG(Core_Logging, Log, TEXT("Core_Logging module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCore_Logging, Core_Logging)