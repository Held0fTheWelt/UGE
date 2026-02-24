// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core_CollectionEditorMode.h"
#include "Core_CollectionEditorModeToolkit.h"
#include "EdModeInteractiveToolsContext.h"
#include "InteractiveToolManager.h"
#include "Core_CollectionEditorModeCommands.h"


//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
// AddYourTool Step 1 - include the header file for your Tools here
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
#include "Tools/Core_CollectionSimpleTool.h"
#include "Tools/Core_CollectionInteractiveTool.h"

// step 2: register a ToolBuilder in FCore_CollectionEditorMode::Enter() below


#define LOCTEXT_NAMESPACE "Core_CollectionEditorMode"

const FEditorModeID UCore_CollectionEditorMode::EM_Core_CollectionEditorModeId = TEXT("EM_Core_CollectionEditorMode");

FString UCore_CollectionEditorMode::SimpleToolName = TEXT("Core_Collection_ActorInfoTool");
FString UCore_CollectionEditorMode::InteractiveToolName = TEXT("Core_Collection_MeasureDistanceTool");


UCore_CollectionEditorMode::UCore_CollectionEditorMode()
{
	FModuleManager::Get().LoadModule("EditorStyle");

	// appearance and icon in the editing mode ribbon can be customized here
	Info = FEditorModeInfo(UCore_CollectionEditorMode::EM_Core_CollectionEditorModeId,
		LOCTEXT("ModeName", "Core_Collection"),
		FSlateIcon(),
		true);
}


UCore_CollectionEditorMode::~UCore_CollectionEditorMode()
{
}


void UCore_CollectionEditorMode::ActorSelectionChangeNotify()
{
}

void UCore_CollectionEditorMode::Enter()
{
	UEdMode::Enter();

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// AddYourTool Step 2 - register the ToolBuilders for your Tools here.
	// The string name you pass to the ToolManager is used to select/activate your ToolBuilder later.
	//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////// 
	const FCore_CollectionEditorModeCommands& SampleToolCommands = FCore_CollectionEditorModeCommands::Get();

	RegisterTool(SampleToolCommands.SimpleTool, SimpleToolName, NewObject<UCore_CollectionSimpleToolBuilder>(this));
	RegisterTool(SampleToolCommands.InteractiveTool, InteractiveToolName, NewObject<UCore_CollectionInteractiveToolBuilder>(this));

	// active tool type is not relevant here, we just set to default
	GetToolManager()->SelectActiveToolType(EToolSide::Left, SimpleToolName);
}

void UCore_CollectionEditorMode::CreateToolkit()
{
	Toolkit = MakeShareable(new FCore_CollectionEditorModeToolkit);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> UCore_CollectionEditorMode::GetModeCommands() const
{
	return FCore_CollectionEditorModeCommands::Get().GetCommands();
}

#undef LOCTEXT_NAMESPACE
