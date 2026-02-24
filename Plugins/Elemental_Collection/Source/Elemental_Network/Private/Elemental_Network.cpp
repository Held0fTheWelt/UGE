#include "Elemental_Network.h"

DEFINE_LOG_CATEGORY(Elemental_Network);

#define LOCTEXT_NAMESPACE "FElemental_Network"

void FElemental_Network::StartupModule()
{
	UE_LOG(Elemental_Network, Display, TEXT("Elemental_Network module has been loaded"));
}

void FElemental_Network::ShutdownModule()
{
	UE_LOG(Elemental_Network, Display, TEXT("Elemental_Network module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FElemental_Network, Elemental_Network)