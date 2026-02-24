// EntityCoreEditor.cpp

#include "EntityCoreEditor.h"

#include "PropertyViewer/PrimaryAssetSetArrayCustomization.h"
#include "EntityCoreEditor_Settings.h"

#include "ISettingsModule.h"
#include "PropertyEditorModule.h"
#include "LevelEditor.h"
#include "EditorUtilitySubsystem.h"
#include "Editor.h"
#include "ToolMenus.h"
#include "Editor/UnrealEd/Public/Selection.h"

DEFINE_LOG_CATEGORY(EntityCoreEditor);

#define LOCTEXT_NAMESPACE "FEntityCoreEditor"

TSharedPtr<FUICommandList> FEntityCoreEditor::CommandList;
FName FEntityCoreEditor::TabID = FName("None");

void FEntityCoreEditor::StartupModule()
{
	// Register Editor Settings
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings(
			"Editor", "War Collection", "Entity Core Editor Settings",
			LOCTEXT("RuntimeSettingsName", "Entity Core Editor Settings"),
			LOCTEXT("RuntimeSettingsDescription", "Entity Core Editor Setup"),
			GetMutableDefault<UEntityCoreEditor_Settings>());
	}

	// Register Property Customization
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout(
		"PrimaryAssetSetArray",
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FPrimaryAssetSetArrayCustomization::MakeInstance));

	// Setup runtime menu extension in World Outliner Actor Context Menu
	if (UToolMenus::IsToolMenuUIEnabled())
	{
		UToolMenus::RegisterStartupCallback(
			FSimpleMulticastDelegate::FDelegate::CreateStatic(&FEntityCoreEditor::RegisterOutlinerMenu));
	}

	UE_LOG(EntityCoreEditor, Log, TEXT("EntityCoreEditor module has been loaded"));
}

void FEntityCoreEditor::ShutdownModule()
{
	// Unregister Editor Settings
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Editor", "War Collection", "Entity Core Editor Settings");
	}

	// Unregister menu (defensive)
	if (UToolMenus::IsToolMenuUIEnabled())
	{
		UToolMenus::UnRegisterStartupCallback(this);
		UToolMenus::UnregisterOwner(this);
	}

	UE_LOG(EntityCoreEditor, Log, TEXT("EntityCoreEditor module has been unloaded"));
}

void FEntityCoreEditor::RegisterOutlinerMenu()
{
	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.ActorContextMenu");

	FToolMenuSection& Section = Menu->FindOrAddSection("ActorOptions");

	Section.AddMenuEntry(
		"EntityCoreEditor_OpenEntityEditor",
		LOCTEXT("OpenEntityEditor_Label", "Open Entity Editor"),
		LOCTEXT("OpenEntityEditor_Tooltip", "Open the runtime Entity Editor"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateStatic(&FEntityCoreEditor::HandleEntityEditorRequest),
			FCanExecuteAction::CreateStatic(&FEntityCoreEditor::IsPlayWorldActive))
	);
}

bool FEntityCoreEditor::IsPlayWorldActive()
{
	return GEditor && GEditor->PlayWorld;
}

void FEntityCoreEditor::HandleEntityEditorRequest()
{
	if (!IsPlayWorldActive())
	{
		UE_LOG(EntityCoreEditor, Warning, TEXT("Editor is not in PIE or SIE mode."));
		return;
	}

	OpenEntityEditor();
}

void FEntityCoreEditor::OpenEntityEditor()
{
	if (!GEditor || !GEditor->PlayWorld)
	{
		UE_LOG(EntityCoreEditor, Warning, TEXT("Not in Play or Simulate mode."));
		return;
	}

	UE_LOG(EntityCoreEditor, Log, TEXT("Opening Entity Editor..."));

	if (UEditorUtilitySubsystem* Subsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>())
	{
		const UEntityCoreEditor_Settings* Settings = GetDefault<UEntityCoreEditor_Settings>();

		if (!Settings || Settings->DefaultEntityEditorWidget.IsNull())
		{
			UE_LOG(EntityCoreEditor, Warning, TEXT("DefaultEntityEditorWidget is not set."));
			return;
		}

		Settings->DefaultEntityEditorWidget.LoadSynchronous();

		if (TabID != FName("None"))
		{
			Subsystem->CloseTabByID(TabID);
			TabID = FName("None");
		}

		Subsystem->SpawnAndRegisterTabAndGetID(Settings->DefaultEntityEditorWidget.Get(), TabID);
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FEntityCoreEditor, EntityCoreEditor)
