#include "Core_GameFeatures.h"

DEFINE_LOG_CATEGORY(Core_GameFeatures);

#define LOCTEXT_NAMESPACE "FCore_GameFeatures"

void FCore_GameFeatures::StartupModule()
{
	UE_LOG(Core_GameFeatures, Display, TEXT("Core_GameFeatures module has been loaded"));
}

void FCore_GameFeatures::ShutdownModule()
{
	UE_LOG(Core_GameFeatures, Display, TEXT("Core_GameFeatures module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCore_GameFeatures, Core_GameFeatures)