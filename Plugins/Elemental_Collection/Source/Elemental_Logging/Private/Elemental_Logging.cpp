#include "Elemental_Logging.h"

DEFINE_LOG_CATEGORY(Elemental_Logging);

#define LOCTEXT_NAMESPACE "FElemental_Logging"

void FElemental_Logging::StartupModule()
{
	UE_LOG(Elemental_Logging, Log, TEXT("Elemental_Logging module has been loaded"));
}

void FElemental_Logging::ShutdownModule()
{
	UE_LOG(Elemental_Logging, Log, TEXT("Elemental_Logging module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FElemental_Logging, Elemental_Logging)