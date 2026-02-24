#include "Elemental_Structure.h"

DEFINE_LOG_CATEGORY(Elemental_Structure);

#define LOCTEXT_NAMESPACE "FElemental_Structure"

void FElemental_Structure::StartupModule()
{
	UE_LOG(Elemental_Structure, Display, TEXT("Elemental_Structure module has been loaded"));
}

void FElemental_Structure::ShutdownModule()
{
	UE_LOG(Elemental_Structure, Display, TEXT("Elemental_Structure module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FElemental_Structure, Elemental_Structure)