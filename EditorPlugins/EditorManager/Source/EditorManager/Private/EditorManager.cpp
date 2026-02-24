/*
 * Copyright (c) 2024 Yves Tanas
 * All Rights Reserved.
 *
 * This file is part of the Collections project.
 *
 * Unauthorized copying of this file, via any medium, is strictly prohibited.
 * Proprietary and confidential.
 *
 * This software may be used only as expressly authorized by the copyright holder.
 * Unless required by applicable law or agreed to in writing, software distributed
 * under this license is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
 *
 * For licensing inquiries, please contact: yves.tanas@example.com
 *
 * Contributors:
 *   Yves Tanas <yves.tanas@example.com>
 *
 * -------------------------------------------------------------------------------
 * File:        [EditorManager.cpp]
 * Created:     [2025-06-12]
 * Description: [Short file/module purpose]
 * -------------------------------------------------------------------------------
 */

#include "EditorManager.h"
#include "EditorManagerStyle.h"
#include "EditorManagerCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "ISettingsModule.h"
#include "ISettingsContainer.h"

static const FName EditorManagerTabName("EditorManager");

#define LOCTEXT_NAMESPACE "FEditorManagerModule"

/**
 * Called after the module is loaded into memory.
 * Responsible for registering styles, commands, menu items, and the plugin's main editor tab.
 */
void FEditorManagerModule::StartupModule()
{
	// Initialize the custom style set and load any textures used by this plugin.
	FEditorManagerStyle::Initialize();
	FEditorManagerStyle::ReloadTextures();

	// Register plugin commands with Unreal's command system.
	FEditorManagerCommands::Register();

	// Create a shared command list for use with toolbar/menu bindings.
	PluginCommands = MakeShareable(new FUICommandList);

	// Map the command for opening the plugin window to a handler.
	PluginCommands->MapAction(
		FEditorManagerCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FEditorManagerModule::PluginButtonClicked),
		FCanExecuteAction());

	// Register a callback to extend menus when the ToolMenus system is initialized.
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FEditorManagerModule::RegisterMenus));

	// Register the plugin's main Nomad tab with the global tab manager.
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(EditorManagerTabName, FOnSpawnTab::CreateRaw(this, &FEditorManagerModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FEditorManagerTabTitle", "EditorManager"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	// Example of extending the Level Editor's toolbar with a custom sub-menu.
	UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu(
		"LevelEditor.LevelEditorToolBar.ModesToolBar");
	FToolMenuSection& ToolbarSection = ToolbarMenu->FindOrAddSection("Toolbar");

	ToolbarSection.AddSubMenu(
		"CoreTools",
		LOCTEXT("CoreToolsLabel", "Handmade Tools"),
		LOCTEXT("CoreToolsTooltip", "Show available custom editor tools"),
		FNewToolMenuDelegate::CreateLambda([](UToolMenu* SubMenu)
			{
				SubMenu->AddSection("ToolsSection", LOCTEXT("ToolsSection", "Tools"));
			}),
		false,
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.GameSettings")
	);

	// Optional: set sort priority for "War Collection" in Project Settings.
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings")) {
		// Holen Sie sich den Project-Container
		TSharedPtr<ISettingsContainer> ProjectContainer = SettingsModule->GetContainer("Project");
		if (ProjectContainer.IsValid()) {
			// Setzen Sie eine sehr niedrige Priorit?t f?r Ihre Kategorie
			ProjectContainer->SetCategorySortPriority(FName(TEXT("War Collection")), -100.0f);
		}
	}

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings")) {
		// Holen Sie sich den Project-Container
		TSharedPtr<ISettingsContainer> ProjectContainer = SettingsModule->GetContainer("Editor");
		if (ProjectContainer.IsValid()) {
			// Setzen Sie eine sehr niedrige Priorit?t f?r Ihre Kategorie
			ProjectContainer->SetCategorySortPriority(FName(TEXT("War Collection")), -100.0f);
		}
	}
}

/**
 * Called during module shutdown to clean up resources.
 * Unregisters menus, commands, styles, and the main editor tab.
 */
void FEditorManagerModule::ShutdownModule()
{
	// Remove registered menu startup callback and owner references.
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);

	// Shutdown the custom style set and unregister commands.
	FEditorManagerStyle::Shutdown();
	FEditorManagerCommands::Unregister();

	// Unregister the Nomad tab.
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(EditorManagerTabName);
}

/**
 * Spawns the content of the plugin's main Nomad tab.
 * Replace the widget content below to create your custom editor UI.
 *
 * @param SpawnTabArgs Arguments for spawning the tab (unused).
 * @return Shared reference to the constructed SDockTab.
 */
TSharedRef<SDockTab> FEditorManagerModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FEditorManagerModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("EditorManager.cpp"))
	);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Example tab content. Replace with your custom widgets.
			SNew(SBox)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
						.Text(WidgetText)
				]
		];
}

/**
 * Handles the event when the plugin's toolbar/menu button is clicked.
 * Invokes the main Nomad tab.
 */
void FEditorManagerModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(EditorManagerTabName);
}

/**
 * Registers custom menus and toolbar buttons for this plugin.
 * Extends both the main menu and the level editor toolbar with new entries.
 */
void FEditorManagerModule::RegisterMenus()
{
	// Ensure this module is marked as the menu owner for proper cleanup.
	FToolMenuOwnerScoped OwnerScoped(this);

	// Extend the main menu "Window" section with a menu entry for this plugin.
	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FEditorManagerCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	// Extend the level editor's toolbar with a new button for this plugin.
	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FEditorManagerCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FEditorManagerModule, EditorManager)
