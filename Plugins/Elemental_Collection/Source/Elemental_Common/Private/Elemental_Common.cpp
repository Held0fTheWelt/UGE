#include "Elemental_Common.h"

DEFINE_LOG_CATEGORY(Elemental_Common);

#define LOCTEXT_NAMESPACE "FElemental_Common"

void FElemental_Common::StartupModule()
{
	UE_LOG(Elemental_Common, Verbose, TEXT("Elemental_Common module has been loaded"));
}

void FElemental_Common::ShutdownModule()
{
	UE_LOG(Elemental_Common, Verbose, TEXT("Elemental_Common module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FElemental_Common, Elemental_Common)