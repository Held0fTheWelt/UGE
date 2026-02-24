// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "Core_Effects.h"

DEFINE_LOG_CATEGORY(Log_CoreEffects);

#define LOCTEXT_NAMESPACE "FCoreEffectsModule"

void FCoreEffectsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FCoreEffectsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For module dependencies, you may call .Unload() on them.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCoreEffectsModule, Core_Effects)
