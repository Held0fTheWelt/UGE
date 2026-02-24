// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorManagerCommands.h"

#define LOCTEXT_NAMESPACE "FEditorManagerModule"

void FEditorManagerCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "EditorManager", "Bring up EditorManager window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
