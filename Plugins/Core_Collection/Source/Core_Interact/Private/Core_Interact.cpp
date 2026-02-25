#include "Core_Interact.h"

DEFINE_LOG_CATEGORY(Core_Interact);

#define LOCTEXT_NAMESPACE "FCore_Interact"

void FCore_Interact::StartupModule()
{
	UE_LOG(Core_Interact, Warning, TEXT("Core_Interact module has been loaded"));
}

void FCore_Interact::ShutdownModule()
{
	UE_LOG(Core_Interact, Warning, TEXT("Core_Interact module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCore_Interact, Core_Interact)