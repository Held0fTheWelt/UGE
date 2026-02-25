#include "Core_Data.h"

DEFINE_LOG_CATEGORY(Core_Data);

#define LOCTEXT_NAMESPACE "FCore_Data"

void FCore_Data::StartupModule()
{
	UE_LOG(Core_Data, Log, TEXT("Core_Data module has been loaded"));
}

void FCore_Data::ShutdownModule()
{
	UE_LOG(Core_Data, Log, TEXT("Core_Data module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCore_Data, Core_Data)