// Copyright Epic Games, Inc. All Rights Reserved.

#include "EntityVisualEditor.h"
#include "EntityVisualEditorStyle.h"
#include "EntityVisualEditorCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

#include "ISettingsModule.h"
#include "EntityVisualEditor_Settings.h"
#include "Modules/ModuleManager.h"

static const FName EntityVisualEditorTabName("EntityVisualEditor");

#define LOCTEXT_NAMESPACE "FEntityVisualEditorModule"

void FEntityVisualEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FEntityVisualEditorStyle::Initialize();
	FEntityVisualEditorStyle::ReloadTextures();

	FEntityVisualEditorCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FEntityVisualEditorCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FEntityVisualEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FEntityVisualEditorModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(EntityVisualEditorTabName, FOnSpawnTab::CreateRaw(this, &FEntityVisualEditorModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FEntityVisualEditorTabTitle", "EntityVisualEditor"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Editor", "War Collection", "Entity Visual Editor Settings",
			LOCTEXT("RuntimeSettingsName", "Entity Visual Editor Settings"), LOCTEXT("RuntimeSettingsDescription", "Entity Visual Editor Setup"),
			GetMutableDefault<UEntityVisualEditor_Settings>());

	}
}

void FEntityVisualEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FEntityVisualEditorStyle::Shutdown();

	FEntityVisualEditorCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(EntityVisualEditorTabName);

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Editor", "War Collection", "Game Feature Interact Settings");
	}
}

TSharedRef<SDockTab> FEntityVisualEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FEntityVisualEditorModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("EntityVisualEditor.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FEntityVisualEditorModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(EntityVisualEditorTabName);
}

void FEntityVisualEditorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FEntityVisualEditorCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FEntityVisualEditorCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEntityVisualEditorModule, EntityVisualEditor)