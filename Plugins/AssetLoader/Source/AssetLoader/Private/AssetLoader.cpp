// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssetLoader.h"

#include "Modules/ModuleManager.h"
#if WITH_EDITOR
#include "ISettingsModule.h"
#endif

#include "AssetLoader_Settings.h"

#define LOCTEXT_NAMESPACE "FAssetLoaderModule"

DEFINE_LOG_CATEGORY(AssetLoader);

void FAssetLoaderModule::StartupModule()
{
	//UpdateEntityDefinitionLookupTable();

	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "War Collection", "Asset Loader",
			LOCTEXT("RuntimeSettingsName", "Asset Loader"), LOCTEXT("RuntimeSettingsDescription", "Setup for Asset Loader Capabilities"),
			GetMutableDefault<UAssetLoader_Settings>());
	}
#endif


	UE_LOG(AssetLoader, Log, TEXT("Elemental_AssetLoader module has been loaded"));
}

void FAssetLoaderModule::ShutdownModule()
{
#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "War Collection", "Asset Loader");
	}
#endif
	UE_LOG(AssetLoader, Log, TEXT("Elemental_AssetLoader module has been unloaded"));
}

void FAssetLoaderModule::UpdateEntityDefinitionLookupTable()
{
	const UAssetLoader_Settings* Settings = GetDefault<UAssetLoader_Settings>();
	if (!Settings)
	{
		UE_LOG(AssetLoader, Error, TEXT("Failed to retrieve Elemental_AssetLoader_Settings. EntityDefinitionLookupTable will not be updated."));
		return;
	}

	UE_LOG(AssetLoader, Log, TEXT("Elemental_AssetLoader module has updated EntityDefinitionLookupTable"));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAssetLoaderModule, AssetLoader)
