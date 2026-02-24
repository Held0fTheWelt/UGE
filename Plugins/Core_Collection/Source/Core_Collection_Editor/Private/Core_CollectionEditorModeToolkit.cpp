// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core_CollectionEditorModeToolkit.h"
#include "Core_CollectionEditorMode.h"
#include "Engine/Selection.h"

#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "Core_CollectionEditorModeToolkit"

FCore_CollectionEditorModeToolkit::FCore_CollectionEditorModeToolkit()
{
}

void FCore_CollectionEditorModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode)
{
	FModeToolkit::Init(InitToolkitHost, InOwningMode);
}

void FCore_CollectionEditorModeToolkit::GetToolPaletteNames(TArray<FName>& PaletteNames) const
{
	PaletteNames.Add(NAME_Default);
}


FName FCore_CollectionEditorModeToolkit::GetToolkitFName() const
{
	return FName("Core_CollectionEditorMode");
}

FText FCore_CollectionEditorModeToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("DisplayName", "Core_CollectionEditorMode Toolkit");
}

#undef LOCTEXT_NAMESPACE
