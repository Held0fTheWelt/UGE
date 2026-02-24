// Fill out your copyright notice in the Description page of Project Settings.


#include "Libs/EditorManagerUtilityLibrary.h"

#include "Editor/EditorEngine.h"

#include "Kismet/KismetSystemLibrary.h"

#include "EditorAssetLibrary.h"
#include "EditorUtilityLibrary.h"

#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"

#include "Subsystems/EditorActorSubsystem.h"

#include "Selection.h"

DEFINE_LOG_CATEGORY_STATIC(Log_EditorManagerUtilityLibrary, Log, All);

void UEditorManagerUtilityLibrary::GetSelectedLevelActors(TArray<AActor*>& OutActors)
{
#if WITH_EDITOR
	OutActors.Reset();

	if (!GEditor) return;

	// Preferred: use the EditorActorSubsystem (supports BP and C++)
	if (UEditorActorSubsystem* ActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>())
	{
		OutActors = ActorSubsystem->GetSelectedLevelActors();
		return;
	}

	// Fallback: read raw USelection
	if (USelection* Sel = GEditor->GetSelectedActors())
	{
		int count = Sel->GetSelectedObjects<AActor>(OutActors);
	}
#endif
}

FDelegateHandle UEditorManagerUtilityLibrary::BindOnSelectionChanged(const FSimpleDelegate& InCallback)
{
	FDelegateHandle Handle;
#if WITH_EDITOR
	if (!GEditor) return Handle;

	if (USelection* Sel = GEditor->GetSelectedActors())
	{
		// Fires whenever the actor selection set changes
		Handle = Sel->SelectionChangedEvent.AddLambda(
			[InCallback](UObject* /*NewSelection*/)
			{
				InCallback.ExecuteIfBound();
			}
		);
	}

	// Optional zusätzlich global lauschen:
	// FEditorDelegates::OnEditorSelectionChanged.AddLambda([InCallback](){ InCallback.ExecuteIfBound(); });
#endif
	return Handle;
}

void UEditorManagerUtilityLibrary::UnbindOnSelectionChanged(FDelegateHandle& Handle)
{
#if WITH_EDITOR
	if (!GEditor || !Handle.IsValid()) return;

	if (USelection* Sel = GEditor->GetSelectedActors())
	{
		Sel->SelectionChangedEvent.Remove(Handle);
		Handle.Reset();
	}
#endif
}

TArray<UObject*> UEditorManagerUtilityLibrary::BeginTransactionAndGetSelectedAssets(const FString& UtilityName, const FString& UtilityDescription, UObject* PrimaryObject)
{
	UE_LOGFMT(Log_EditorManagerUtilityLibrary, VeryVerbose, "{Time}: {Line} {Class}: BeginTransactionAndGetSelectedAssets called with UtilityName: {UtilityName}, UtilityDescription: {UtilityDescription}, PrimaryObject: {PrimaryObject}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("UtilityName", UtilityName),
		("UtilityDescription", UtilityDescription),
		("PrimaryObject", PrimaryObject ? *PrimaryObject->GetName() : TEXT("None"))
	);

	if (UtilityName.IsEmpty())
	{
		UE_LOGFMT(Log_EditorManagerUtilityLibrary, Warning, "{Time}: {Line} {Class}: UtilityName is empty, cannot execute action.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME)
		);
		return {};
	}

	if (UtilityDescription.IsEmpty())
	{
		UE_LOGFMT(Log_EditorManagerUtilityLibrary, VeryVerbose, "{Time}: {Line} {Class}: UtilityDescription is empty, cannot execute action.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME)
		);
	}

	UE_LOGFMT(Log_EditorManagerUtilityLibrary, Verbose, "{Time}: {Line} {Class}: Begin Transaction for Utility: {UtilityName}, Description: {UtilityDescription}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("UtilityName", UtilityName),
		("UtilityDescription", UtilityDescription)
	);

	UKismetSystemLibrary::BeginTransaction(UtilityName, FText::FromString(UtilityDescription), PrimaryObject);

	TArray<UObject*> SelectedAssets = TArray<UObject*>();

	if (!PrimaryObject)
	{
		SelectedAssets = UEditorUtilityLibrary::GetSelectedAssets();
		UE_LOGFMT(Log_EditorManagerUtilityLibrary, Verbose, "{Time}: {Line} {Class}: No PrimaryObject provided, using selected assets: {SelectedAssets}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("SelectedAssets", SelectedAssets.Num() > 0 ? FString::JoinBy(SelectedAssets, TEXT(", "), [](const UObject* Asset) { return Asset ? Asset->GetName() : TEXT("None"); }) : TEXT("None"))
		);
	}
	else
	{
		SelectedAssets = UEditorUtilityLibrary::GetSelectedAssetsOfClass(PrimaryObject->GetClass());
		UE_LOGFMT(Log_EditorManagerUtilityLibrary, Verbose, "{Time}: {Line} {Class}: Using PrimaryObject: {PrimaryObject}, SelectedAssets: {SelectedAssets}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("PrimaryObject", PrimaryObject ? *PrimaryObject->GetName() : TEXT("None")),
			("SelectedAssets", SelectedAssets.Num() > 0 ? FString::JoinBy(SelectedAssets, TEXT(", "), [](const UObject* Asset) { return Asset ? Asset->GetName() : TEXT("None"); }) : TEXT("None"))
		);
	}

	return SelectedAssets;
}

void UEditorManagerUtilityLibrary::CheckoutSaveAndEndTransaction(const TArray<UObject*>& ObjectsToCheckout)
{
	if (ObjectsToCheckout.Num() == 0)
	{
		UE_LOGFMT(Log_EditorManagerUtilityLibrary, Warning, "{Time}: {Line} {Class}: No assets selected to execute undoable action.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME)
		);
		return;
	}

	if (UEditorAssetLibrary::CheckoutLoadedAssets(ObjectsToCheckout))
	{
		UE_LOGFMT(Log_EditorManagerUtilityLibrary, Verbose, "{Time}: {Line} {Class}: Successfully checked out assets.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME)
		);
	}
	else
	{
		UE_LOGFMT(Log_EditorManagerUtilityLibrary, Verbose, "{Time}: {Line} {Class}: Failed to check out assets.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME)
		);
	}

	UEditorAssetLibrary::SaveLoadedAssets(ObjectsToCheckout);
	UE_LOGFMT(Log_EditorManagerUtilityLibrary, Verbose, "{Time}: {Line} {Class}: Saved loaded assets.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME)
	);

	UKismetSystemLibrary::EndTransaction();
	UE_LOGFMT(Log_EditorManagerUtilityLibrary, Verbose, "{Time}: {Line} {Class}: Ended transaction.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME)
	);
}