// Copyright Epic Games, Inc. All Rights Reserved.

#include "VehiclesEditor.h"
#include "VehiclesEditorStyle.h"
#include "VehiclesEditorCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

static const FName VehiclesEditorTabName("VehiclesEditor");

#define LOCTEXT_NAMESPACE "FVehiclesEditorModule"

void FVehiclesEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FVehiclesEditorStyle::Initialize();
	FVehiclesEditorStyle::ReloadTextures();

	FVehiclesEditorCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FVehiclesEditorCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FVehiclesEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FVehiclesEditorModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(VehiclesEditorTabName, FOnSpawnTab::CreateRaw(this, &FVehiclesEditorModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FVehiclesEditorTabTitle", "VehiclesEditor"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	FToolMenuOwnerScoped OwnerScoped(this);

	// Add to Main Menu
	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
	FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
	Section.AddMenuEntryWithCommandList(FVehiclesEditorCommands::Get().OpenPluginWindow, PluginCommands);
	UToolMenu* SubMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.ModesToolBar.CoreTools");
	FToolMenuSection& SubMenuSection = SubMenu->FindOrAddSection("ToolsSection");
	SubMenuSection.AddMenuEntry(
		"OpenVehiclesEditor",
		LOCTEXT("VehiclesEditor_Tab", "Vehicles Editor"),
		LOCTEXT("VehiclesEditor_Tab_Tooltip", "Open Vehicles Editor"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateStatic(&FVehiclesEditorModule::PluginButtonClickedStatic))
	);
	//ToolbarSection.AddSubMenu(
	//	"Core Tools",
	//	LOCTEXT("CoreToolsLabel", "Handmade Tools"),
	//	LOCTEXT("CoreToolsTooltip", "Show available custom editor tools"),
	//	FNewToolMenuDelegate::CreateLambda([](UToolMenu* SubMenu)
	//		{
	//			// Du holst dir hier explizit eine Section aus dem SubMenu:
	//			FToolMenuSection& SubMenuSection = SubMenu->AddSection("ToolsSection", LOCTEXT("ToolsSection", "Tools"));

	//			// Einträge von Plugin 1
	//			SubMenuSection.AddMenuEntry(
	//				"OpenVehiclesEditor",
	//				LOCTEXT("VehiclesEditor_Tab", "Vehicles Debug Editor"),
	//				LOCTEXT("VehiclesEditor_Tab_Tooltip", "Open Vehicles Debug Editor"),
	//				FSlateIcon(),
	//				FUIAction(FExecuteAction::CreateStatic(&FVehiclesEditorModule::PluginButtonClickedStatic))
	//			);
	//			// ...weitere Plugins können sich hier einklinken
	//		}),
	//	false,
	//	FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.GameSettings")
}

void FVehiclesEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FVehiclesEditorStyle::Shutdown();

	FVehiclesEditorCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(VehiclesEditorTabName);
}

TSharedRef<SDockTab> FVehiclesEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FVehiclesEditorModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("VehiclesEditor.cpp"))
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

void FVehiclesEditorModule::PluginButtonClicked()
{
	// This function will be bound to Command (by default it will bring up plugin window)
	FGlobalTabmanager::Get()->TryInvokeTab(VehiclesEditorTabName);
}
void FVehiclesEditorModule::PluginButtonClickedStatic()
{
	FGlobalTabmanager::Get()->TryInvokeTab(VehiclesEditorTabName);
}

void FVehiclesEditorModule::OpenLogLevelEditorTab()
{
}

void FVehiclesEditorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FVehiclesEditorCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FVehiclesEditorCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FVehiclesEditorModule, VehiclesEditor)