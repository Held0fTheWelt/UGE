#include "Elemental_MessageSystem.h"

DEFINE_LOG_CATEGORY(Elemental_MessageSystem);

void FElemental_MessageSystem::StartupModule()
{
	UE_LOG(Elemental_MessageSystem, Verbose, TEXT("Elemental_MessageSystem module has been initialized"));
}

void FElemental_MessageSystem::ShutdownModule()
{
}

IMPLEMENT_MODULE(FElemental_MessageSystem, Elemental_MessageSystem)
