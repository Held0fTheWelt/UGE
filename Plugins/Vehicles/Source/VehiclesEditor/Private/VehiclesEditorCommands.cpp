// Copyright Epic Games, Inc. All Rights Reserved.

#include "VehiclesEditorCommands.h"

#define LOCTEXT_NAMESPACE "FVehiclesEditorModule"

void FVehiclesEditorCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "VehiclesEditor", "Bring up VehiclesEditor window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
