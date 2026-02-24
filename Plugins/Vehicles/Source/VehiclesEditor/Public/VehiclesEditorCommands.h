// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Framework/Commands/Commands.h"
#include "VehiclesEditorStyle.h"

class FVehiclesEditorCommands : public TCommands<FVehiclesEditorCommands>
{
public:

	FVehiclesEditorCommands()
		: TCommands<FVehiclesEditorCommands>(TEXT("VehiclesEditor"), NSLOCTEXT("Contexts", "VehiclesEditor", "VehiclesEditor Plugin"), NAME_None, FVehiclesEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};