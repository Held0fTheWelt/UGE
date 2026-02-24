// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/UndoableAssetActionUtility.h"

#include "Kismet/KismetSystemLibrary.h"

#include "Libs/EditorManagerUtilityLibrary.h"

#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"

#include "Utility/EditorUtilityTask_PrepareUndo.h"

#include "EditorUtilityTask.h"
#include "EditorUtilitySubsystem.h"

DEFINE_LOG_CATEGORY(Log_UndoableAssetActionUtility);

bool UUndoableAssetActionUtility::EditAsset_Implementation(UObject* Object)
{
	UE_LOGFMT(Log_UndoableAssetActionUtility, VeryVerbose, "{Time}: {Line} {Class}: EditAsset Implementation called for Object {Object}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Object", Object ? *Object->GetFName().ToString() : TEXT("None"))
	);

	return true; // Default implementation does nothing, can be overridden in derived classes
}

bool UUndoableAssetActionUtility::EditAllAssets_Implementation(const TArray<class UObject*>& Objects)
{
	return true; // Default implementation does nothing, can be overridden in derived classes
}

void UUndoableAssetActionUtility::ExecuteUndoableAction(UObject* PrimaryObject)
{
	bool Success = false;

	PrepareUndoTask = NewObject<UEditorUtilityTask_PrepareUndo>();
	if (!PrepareUndoTask)
	{
		PrepareUndoTask->SetTaskNotificationText(FText::FromString("PrepareUndoTask is null, cannot execute action."));
		return;
	}

	PrepareUndoTask->UtilityName = UtilityName;
	PrepareUndoTask->UtilityDescription = UtilityDescription;

	UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();

	if(!EditorUtilitySubsystem)
	{
		PrepareUndoTask->SetTaskNotificationText(FText::FromString("EditorUtilitySubsystem is null, cannot execute action."));
		return;
	}


	EditorUtilitySubsystem->RegisterAndExecuteTask(PrepareUndoTask, nullptr);

	PrepareUndoTask->SetTaskNotificationText(FText::FromString("Beginning Transaction"));

	TArray<UObject*> SelectedAssets = UEditorManagerUtilityLibrary::BeginTransactionAndGetSelectedAssets(
		UtilityName,
		UtilityDescription,
		PrimaryObject
	);

	if(SelectedAssets.Num() == 0)
	{
		PrepareUndoTask->SetTaskNotificationText(FText::FromString("No assets selected for undoable action."));
		return;
	}

	PrepareUndoTask->SetTaskNotificationText(FText::FromString("Touching Assets"));

	for(UObject* Asset : SelectedAssets)
	{
		if(!Asset)
		{
			PrepareUndoTask->SetTaskNotificationText(FText::FromString("Asset is null, skipping."));
			continue;
		}

		if(!CanExecuteUndoableAction(Asset))
		{
			PrepareUndoTask->SetTaskNotificationText(FText::FromString(FString::Printf(TEXT("Cannot execute undoable action on asset %s. Skipping."), *Asset->GetName())));
			continue;
		}

		UE_LOGFMT(Log_UndoableAssetActionUtility, VeryVerbose, "{Time}: {Line} {Class}: Executing undoable action on asset {Asset}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Asset", Asset ? *Asset->GetName() : TEXT("None"))
		);

		UKismetSystemLibrary::TransactObject(Asset);

		UE_LOGFMT(Log_UndoableAssetActionUtility, VeryVerbose, "{Time}: {Line} {Class}: Editing asset {Asset}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Asset", Asset ? *Asset->GetName() : TEXT("None"))
		);

		if(EditAsset(Asset))
		{
			PrepareUndoTask->SetTaskNotificationText(FText::FromString(FString::Printf(TEXT("Successfully edited asset %s."), *Asset->GetName())));
		}
		else
		{
			PrepareUndoTask->SetTaskNotificationText(FText::FromString(FString::Printf(TEXT("Failed to edit asset %s."), *Asset->GetName())));
		}

		UE_LOGFMT(Log_UndoableAssetActionUtility, VeryVerbose, "{Time}: {Line} {Class}: Finished editing asset {Asset}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Asset", Asset ? *Asset->GetName() : TEXT("None"))
		);
	}

	UE_LOGFMT(Log_UndoableAssetActionUtility, Verbose, "{Time}: {Line} {Class}: Checkout loaded assets for SelectedAssets: {SelectedAssets}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("SelectedAssets", SelectedAssets.Num() > 0 ? FString::JoinBy(SelectedAssets, TEXT(", "), [](const UObject* Asset) { return Asset ? Asset->GetName() : TEXT("None"); }) : TEXT("None"))
	);

	PrepareUndoTask->SetTaskNotificationText(FText::FromString("Edit All Assets"));
	Success = EditAllAssets(SelectedAssets);

	if(!Success)
	{
		PrepareUndoTask->SetTaskNotificationText(FText::FromString("Edit All Assets failed."));
		return;
	}

	PrepareUndoTask->SetTaskNotificationText(FText::FromString("Checkout and Save All Assets"));
	UEditorManagerUtilityLibrary::CheckoutSaveAndEndTransaction(SelectedAssets);

	PrepareUndoTask = nullptr; // Reset the task after execution
}

bool UUndoableAssetActionUtility::CanExecuteUndoableAction(UObject* PrimaryObject) const
{
    return true;
}
