#include "Core_Interfaces.h"

DEFINE_LOG_CATEGORY(Core_Interfaces);

#define LOCTEXT_NAMESPACE "FCore_Interfaces"

void FCore_Interfaces::StartupModule()
{
	UE_LOG(Core_Interfaces, Log, TEXT("Core_Interfaces module has been loaded"));
}

void FCore_Interfaces::ShutdownModule()
{
	UE_LOG(Core_Interfaces, Log, TEXT("Core_Interfaces module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCore_Interfaces, Core_Interfaces)