// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/AssetSetActionUtility.h"

#include "DataAssets/Primary/PrimaryDataAssetSet.h"

#include "Interfaces/AssetEditorInterface.h"

#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"

#include "EntityCoreEditor_Settings.h"

#include "EditorUtilitySubsystem.h"

DEFINE_LOG_CATEGORY(Log_AssetSetActionUtility);

UAssetSetActionUtility::UAssetSetActionUtility()
{
	SupportedClasses.Add(UPrimaryDataAssetSet::StaticClass());

	UtilityName = TEXT("Asset Set Action Utility");
	UtilityDescription = TEXT("Utility for performing actions on Asset Sets, such as opening the Asset Set Editor.");
}

void UAssetSetActionUtility::OpenAssetSetEditor()
{
	// This function is intended to open the Asset Set Editor UI.
	// The actual implementation would depend on the specific editor functionality and UI framework used.
	// For now, we can log a message or trigger an event to open the editor.
	UE_LOGFMT(Log_AssetSetActionUtility, Log, "{Time}: {Line} {Class}: Setting Material Parent...",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME));

	Mode = 1; // Set mode to indicate changing material parent

	ExecuteUndoableAction(nullptr);
}

bool UAssetSetActionUtility::EditAllAssets_Implementation(const TArray<class UObject*>& Objects)
{
	Super::EditAllAssets_Implementation(Objects);

	if (Objects.Num() == 0)
	{
		UE_LOGFMT(Log_AssetSetActionUtility, Warning, "{Time}: {Line} {Class}: No objects provided for EditAllAssets.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME)
		);
		return false;
	}

	class UEditorUtilityWidgetBlueprint* AssetActionsWidget = GetAssetActionsEditorUtilityWidget();

	if(!AssetActionsWidget)
	{
		UE_LOGFMT(Log_AssetSetActionUtility, Warning, "{Time}: {Line} {Class}: AssetActionsWidget is not set.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME)
		);
		return false;
	}

	UE_LOGFMT(Log_AssetSetActionUtility, VeryVerbose, "{Time}: {Line} {Class}: EditAllAssets Implementation called for Objects {Objects}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Objects", Objects.Num() > 0 ? *Objects[0]->GetFName().ToString() : TEXT("None"))
	);
	
	UEditorUtilityWidget* AssetManagerEditor = RunEditorUtilityWidget(AssetActionsWidget);

	if (!AssetManagerEditor)
	{
		UE_LOGFMT(Log_AssetSetActionUtility, Warning, "{Time}: {Line} {Class}: Failed to run AssetActionsEditorUtilityWidget.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME)
		);
		return false;
	}

	// Set the current primary data assets in the editor utility widget
	if(AssetManagerEditor->GetClass()->ImplementsInterface(UAssetEditorInterface::StaticClass()))
	{
		IAssetEditorInterface::Execute_SetCurrentPrimaryDataAssets(AssetManagerEditor, Objects);
	}
	else
	{
		UE_LOGFMT(Log_AssetSetActionUtility, Warning, "{Time}: {Line} {Class}: AssetManagerEditor does not implement IAssetEditorInterface.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME)
		);
		return false;
	}

	return true;
}

UEditorUtilityWidget* UAssetSetActionUtility::RunEditorUtilityWidget(UEditorUtilityWidgetBlueprint* ActionsWidget)
{
	if (!GEditor) return nullptr;

	UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();
	if (!EditorUtilitySubsystem)
	{
		UE_LOGFMT(Log_AssetSetActionUtility, Warning, "{Time}: {Line} {Class}: EditorUtilitySubsystem is not available.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME)
		);
		return nullptr;
	}

	if (!ActionsWidget)
	{
		UE_LOGFMT(Log_AssetSetActionUtility, Warning, "{Time}: {Line} {Class}: ActionsWidget is not set.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME)
		);
		return nullptr;
	}

	if (TabID != FName("None"))
	{
		// Wenn der TabID bereits gesetzt ist, entfernen wir den Tab
		UE_LOGFMT(Log_AssetSetActionUtility, Log, "{Time}: {Line} {Class}: Unregistering existing tab with ID {TabID}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("TabID", TabID),
			("Time", GET_CURRENT_TIME)
		);
		// Wenn der Tab bereits existiert, entfernen wir ihn
		EditorUtilitySubsystem->CloseTabByID(TabID);
		TabID = FName("None"); // Setze TabID zurück, um einen neuen Tab zu erstellen
	}

	// Das Widget starten (öffnet ein Editor-Tab mit deinem Widget)
	UEditorUtilityWidget* CreatedWidget = EditorUtilitySubsystem->SpawnAndRegisterTabAndGetID(ActionsWidget, TabID);
	if (!CreatedWidget)
	{
		UE_LOGFMT(Log_AssetSetActionUtility, Warning, "{Time}: {Line} {Class}: Failed to spawn and register tab for ActionsWidget.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME)
		);
		return nullptr;
	}

	return CreatedWidget;
}

class UEditorUtilityWidgetBlueprint* UAssetSetActionUtility::GetAssetActionsEditorUtilityWidget() const
{
	const UEntityCoreEditor_Settings* Settings = GetDefault<UEntityCoreEditor_Settings>();

	if (!Settings)
	{
		UE_LOGFMT(Log_AssetSetActionUtility, Warning, "{Time}: {Line} {Class}: EntityCoreEditor_Settings is not available.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME)
		);
		return nullptr;
	}

	TSoftObjectPtr<UEditorUtilityWidgetBlueprint> AssetActionsWidget = Settings->DefaultAssetEditorWidget;

	if (AssetActionsWidget.IsNull())
	{
		UE_LOGFMT(Log_AssetSetActionUtility, Warning, "{Time}: {Line} {Class}: DefaultAssetEditorWidget is not set in EntityCoreEditor_Settings.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME)
		);
		return nullptr;
	}

	if (!AssetActionsWidget.IsValid())
	{
		UE_LOGFMT(Log_AssetSetActionUtility, Display, "{Time}: {Line} {Class}: DefaultAssetEditorWidget is not valid.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME)
		);
		AssetActionsWidget.LoadSynchronous();
	}

	return AssetActionsWidget.Get();
}
