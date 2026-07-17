// Copyright Epic Games, Inc. All Rights Reserved.

#include "InputSystem.h"
#include "Modules/ModuleManager.h"

#include "Input_Settings.h"
#if WITH_EDITOR
#include "ISettingsModule.h"
#endif

#define LOCTEXT_NAMESPACE "FInputSystemModule"

void FInputSystemModule::StartupModule()
{
#if WITH_EDITOR
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "War Collection", "Input Control Settings",
			LOCTEXT("RuntimeSettingsName", "Input Control Settings"), LOCTEXT("RuntimeSettingsDescription", "Input DataAsset Control Setup"),
			GetMutableDefault<UInput_Settings>());
	}
#endif
}

void FInputSystemModule::ShutdownModule()
{
#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "War Collection", "Input Control Settings");
	}
#endif
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FInputSystemModule, InputSystem)
