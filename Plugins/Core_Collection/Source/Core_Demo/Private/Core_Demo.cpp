#include "Core_Demo.h"

DEFINE_LOG_CATEGORY(Core_Demo);

#define LOCTEXT_NAMESPACE "FCore_Demo"

void FCore_Demo::StartupModule()
{
	UE_LOG(Core_Demo, Log, TEXT("Core_Demo module has been loaded"));
}

void FCore_Demo::ShutdownModule()
{
	UE_LOG(Core_Demo, Log, TEXT("Core_Demo module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCore_Demo, Core_Demo)