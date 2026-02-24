#include "Elemental_Interfaces.h"

DEFINE_LOG_CATEGORY(Elemental_Interfaces);

#define LOCTEXT_NAMESPACE "FElemental_Interfaces"

void FElemental_Interfaces::StartupModule()
{
	UE_LOG(Elemental_Interfaces, Log, TEXT("Elemental_Interfaces module has been loaded"));
}

void FElemental_Interfaces::ShutdownModule()
{
	UE_LOG(Elemental_Interfaces, Log, TEXT("Elemental_Interfaces module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FElemental_Interfaces, Elemental_Interfaces)