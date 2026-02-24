#include "Core_Extras.h"

DEFINE_LOG_CATEGORY(Core_Extras);

#define LOCTEXT_NAMESPACE "FCore_Extras"

void FCore_Extras::StartupModule()
{
	UE_LOG(Core_Extras, Log, TEXT("Core_Extras module has been loaded"));
}

void FCore_Extras::ShutdownModule()
{
	UE_LOG(Core_Extras, Log, TEXT("Core_Extras module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCore_Extras, Core_Extras)