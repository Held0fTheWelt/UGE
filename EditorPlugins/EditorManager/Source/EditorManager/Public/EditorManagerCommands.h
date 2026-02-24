// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Framework/Commands/Commands.h"
#include "EditorManagerStyle.h"

class FEditorManagerCommands : public TCommands<FEditorManagerCommands>
{
public:

	FEditorManagerCommands()
		: TCommands<FEditorManagerCommands>(TEXT("EditorManager"), NSLOCTEXT("Contexts", "EditorManager", "EditorManager Plugin"), NAME_None, FEditorManagerStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};