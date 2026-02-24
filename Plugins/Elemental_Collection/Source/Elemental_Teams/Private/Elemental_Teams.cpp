#include "Elemental_Teams.h"

DEFINE_LOG_CATEGORY(Elemental_Teams);

#define LOCTEXT_NAMESPACE "FElemental_Teams"

void FElemental_Teams::StartupModule()
{
	UE_LOG(Elemental_Teams, Verbose, TEXT("Elemental_Teams module has been loaded"));
}

void FElemental_Teams::ShutdownModule()
{
	UE_LOG(Elemental_Teams, Verbose, TEXT("Elemental_Teams module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FElemental_Teams, Elemental_Teams)