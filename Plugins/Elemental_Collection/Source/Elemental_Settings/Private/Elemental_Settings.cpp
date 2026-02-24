#include "Elemental_Settings.h"

DEFINE_LOG_CATEGORY(Elemental_Settings);

#define LOCTEXT_NAMESPACE "FElemental_Settings"

void FElemental_Settings::StartupModule()
{
	UE_LOG(Elemental_Settings, Verbose, TEXT("Elemental_Settings module has been loaded"));
}

void FElemental_Settings::ShutdownModule()
{
	UE_LOG(Elemental_Settings, Verbose, TEXT("Elemental_Settings module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FElemental_Settings, Elemental_Settings)