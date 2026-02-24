// Copyright Epic Games, Inc. All Rights Reserved.

#include "EntityVisualEditorCommands.h"

#define LOCTEXT_NAMESPACE "FEntityVisualEditorModule"

void FEntityVisualEditorCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "EntityVisualEditor", "Bring up EntityVisualEditor window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
