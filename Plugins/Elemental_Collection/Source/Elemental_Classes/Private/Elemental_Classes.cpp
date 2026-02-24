#include "Elemental_Classes.h"

DEFINE_LOG_CATEGORY(Elemental_Classes);

#define LOCTEXT_NAMESPACE "FElemental_Classes"

void FElemental_Classes::StartupModule()
{
	UE_LOG(Elemental_Classes, Log, TEXT("Elemental_Classes module has been loaded"));
}

void FElemental_Classes::ShutdownModule()
{
	UE_LOG(Elemental_Classes, Log, TEXT("Elemental_Classes module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FElemental_Classes, Elemental_Classes)