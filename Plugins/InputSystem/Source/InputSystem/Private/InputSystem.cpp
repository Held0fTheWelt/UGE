// Copyright Epic Games, Inc. All Rights Reserved.

#include "InputSystem.h"
#include "Modules/ModuleManager.h"

#include "Input_Settings.h"
#include "PropertyEditorModule.h"
#include "ISettingsModule.h"

#define LOCTEXT_NAMESPACE "FInputSystemModule"

void FInputSystemModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "War Collection", "Input Control Settings",
			LOCTEXT("RuntimeSettingsName", "Input Control Settings"), LOCTEXT("RuntimeSettingsDescription", "Input DataAsset Control Setup"),
			GetMutableDefault<UInput_Settings>());
	}
}

void FInputSystemModule::ShutdownModule()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "War Collection", "Input Control Settings");
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FInputSystemModule, InputSystem)