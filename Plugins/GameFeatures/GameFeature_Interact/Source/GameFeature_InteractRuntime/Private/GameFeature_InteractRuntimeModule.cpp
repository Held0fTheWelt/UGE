// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameFeature_InteractRuntimeModule.h"

#define LOCTEXT_NAMESPACE "FGameFeature_InteractRuntimeModule"

void FGameFeature_InteractRuntimeModule::StartupModule()
{
	// This code will execute after your module is loaded into memory;
	// the exact timing is specified in the .uplugin file per-module
}

void FGameFeature_InteractRuntimeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.
	// For modules that support dynamic reloading, we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGameFeature_InteractRuntimeModule, GameFeature_InteractRuntime)
