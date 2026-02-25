#include "Core_Classes.h"

DEFINE_LOG_CATEGORY(Core_Classes);

#define LOCTEXT_NAMESPACE "FCore_Classes"

void FCore_Classes::StartupModule()
{
	UE_LOG(Core_Classes, Log, TEXT("Core_Classes module has been loaded"));
}

void FCore_Classes::ShutdownModule()
{
	UE_LOG(Core_Classes, Log, TEXT("Core_Classes module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCore_Classes, Core_Classes)