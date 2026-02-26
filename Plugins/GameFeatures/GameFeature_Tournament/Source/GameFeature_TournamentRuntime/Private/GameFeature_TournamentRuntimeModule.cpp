// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameFeature_TournamentRuntimeModule.h"

#define LOCTEXT_NAMESPACE "FGameFeature_TournamentRuntimeModule"

void FGameFeature_TournamentRuntimeModule::StartupModule()
{
	// This code will execute after your module is loaded into memory;
	// the exact timing is specified in the .uplugin file per-module
}

void FGameFeature_TournamentRuntimeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.
	// For modules that support dynamic reloading, we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGameFeature_TournamentRuntimeModule, GameFeature_TournamentRuntime)
