#include "Elemental_Data.h"

DEFINE_LOG_CATEGORY(Elemental_Data);

#define LOCTEXT_NAMESPACE "FElemental_Data"

void FElemental_Data::StartupModule()
{
	UE_LOG(Elemental_Data, Log, TEXT("Elemental_Data module has been loaded"));
}

void FElemental_Data::ShutdownModule()
{
	UE_LOG(Elemental_Data, Log, TEXT("Elemental_Data module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FElemental_Data, Elemental_Data)