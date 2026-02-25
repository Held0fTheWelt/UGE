// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core_CollectionEditorModeCommands.h"
#include "Core_CollectionEditorMode.h"
#include "Styling/AppStyle.h"

#define LOCTEXT_NAMESPACE "Core_CollectionEditorModeCommands"

FCore_CollectionEditorModeCommands::FCore_CollectionEditorModeCommands()
	: TCommands<FCore_CollectionEditorModeCommands>("Core_CollectionEditorMode",
		NSLOCTEXT("Core_CollectionEditorMode", "Core_CollectionEditorModeCommands", "Core_Collection Editor Mode"),
		NAME_None,
		FAppStyle::GetAppStyleSetName())
{
}

void FCore_CollectionEditorModeCommands::RegisterCommands()
{
	TArray <TSharedPtr<FUICommandInfo>>& ToolCommands = Commands.FindOrAdd(NAME_Default);

	UI_COMMAND(SimpleTool, "Show Actor Info", "Opens message box with info about a clicked actor", EUserInterfaceActionType::Button, FInputChord());
	ToolCommands.Add(SimpleTool);

	UI_COMMAND(InteractiveTool, "Measure Distance", "Measures distance between 2 points (click to set origin, shift-click to set end point)", EUserInterfaceActionType::ToggleButton, FInputChord());
	ToolCommands.Add(InteractiveTool);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> FCore_CollectionEditorModeCommands::GetCommands()
{
	return FCore_CollectionEditorModeCommands::Get().Commands;
}

#undef LOCTEXT_NAMESPACE
