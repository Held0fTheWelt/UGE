// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Framework/Commands/Commands.h"
#include "EntityVisualEditorStyle.h"

class FEntityVisualEditorCommands : public TCommands<FEntityVisualEditorCommands>
{
public:

	FEntityVisualEditorCommands()
		: TCommands<FEntityVisualEditorCommands>(TEXT("EntityVisualEditor"), NSLOCTEXT("Contexts", "EntityVisualEditor", "EntityVisualEditor Plugin"), NAME_None, FEntityVisualEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};