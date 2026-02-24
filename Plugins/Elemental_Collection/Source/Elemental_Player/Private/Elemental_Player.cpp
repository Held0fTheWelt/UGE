#include "Elemental_Player.h"

DEFINE_LOG_CATEGORY(Elemental_Player);

#define LOCTEXT_NAMESPACE "FElemental_Player"

void FElemental_Player::StartupModule()
{
	UE_LOG(Elemental_Player, Verbose, TEXT("Elemental_Player module has been loaded"));
}

void FElemental_Player::ShutdownModule()
{
	UE_LOG(Elemental_Player, Verbose, TEXT("Elemental_Player module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FElemental_Player, Elemental_Player)