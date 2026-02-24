// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/AssetActionsEditorUtilityWidget.h"

#include "Engine/AssetManager.h"

#include "Components/Button.h"
#include "Components/DetailsView.h"
#include "Components/TextBlock.h"

#include "DataAssets/PreLoadingDataAsset.h"
#include "DataAssets/Primary/PrimaryDataAssetSet.h"
#include "PropertyViewer/PrimaryAssetSetArrayCustomization.h"

#include "Libs/EntityCoreEditorFunctionLibrary.h"

#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"

#include "EditorUtilityWidgetComponents.h"

#include "EditorUtilitySubsystem.h"

DEFINE_LOG_CATEGORY(Log_AssetActionsEditorUtilityWidget);

UAssetActionsEditorUtilityWidget::UAssetActionsEditorUtilityWidget()
{
	if(PrimaryAssetSetList)
	{
		PrimaryAssetSetList->CategoriesToShow.Add(FName("PrimaryAssets"));
	}
}

void UAssetActionsEditorUtilityWidget::SetCurrentAssetView()
{
	UE_LOGFMT(Log_AssetActionsEditorUtilityWidget, Verbose, "{Time}: {Line} {Class}: Set Current Asset View.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME)
	);

	if (PrimaryAssetSet.CurrentPrimaryDataAssets.Num() == 0)
	{
		UE_LOGFMT(Log_AssetActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: No valid primary data assets provided.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME)
		);
		return;
	}

	if (PrimaryAssetSet.SetIndex >= PrimaryAssetSet.CurrentPrimaryDataAssets.Num())
	{
		UE_LOGFMT(Log_AssetActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: SetIndex out of Bounds. Resetting to 0",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME)
		);
		PrimaryAssetSet.SetIndex = 0;
	}

	UPrimaryDataAssetSet* CurrentSet = PrimaryAssetSet.CurrentPrimaryDataAssets[PrimaryAssetSet.SetIndex].Get();

	if (!CurrentSet)
	{
		UE_LOGFMT(Log_AssetActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: Current Primary Data Asset Set is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME)
		);
		return;
	}

	TArray<TSoftObjectPtr<class UPreLoadingDataAsset>> AssetSet = CurrentSet->AssetSet;

	if (AssetSet.Num() == 0)
	{
		UE_LOGFMT(Log_AssetActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: No assets found in the current Primary Data Asset Set.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME)
		);
		return;
	}

	UE_LOGFMT(Log_AssetActionsEditorUtilityWidget, Log, "{Time}: {Line} {Class}: Setting Current Asset View with {Count} assets.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Count", AssetSet.Num()),
		("Time", GET_CURRENT_TIME)
	);

	for (int32 i = 0; i < AssetSet.Num(); i++)
	{
		if(AssetSet[i].IsNull())
		{
			UE_LOGFMT(Log_AssetActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: Asset at index {Index} is null.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Index", i),
				("Time", GET_CURRENT_TIME)
			);
			continue;
		}

		// Assuming UPreLoadingDataAsset is a valid class derived from UDataAsset
		if (!AssetSet[i].IsValid())
		{
			UE_LOGFMT(Log_AssetActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: Asset at index {Index} is not valid.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Index", i),
				("Time", GET_CURRENT_TIME)
			);
			AssetSet[i].LoadSynchronous();
		}

		UPreLoadingDataAsset* DataAsset = AssetSet[i].Get();

		if (!DataAsset)
		{
			UE_LOGFMT(Log_AssetActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: Data Asset at index {Index} is null.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Index", i),
				("Time", GET_CURRENT_TIME)
			);
			continue;
		}

		UtilityTreeView_Tool->AddItem(DataAsset);
		UtilityTreeView->AddItem(DataAsset);
	}
}

void UAssetActionsEditorUtilityWidget::SetCurrentPrimaryDataAssets_Implementation(const TArray<class UObject*>& InPrimaryDataAssets)
{
	UE_LOGFMT(Log_AssetActionsEditorUtilityWidget, Verbose, "{Time}: {Line} {Class}: Set Current Primary Data Assets.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME)
	);

	PrimaryAssetSet.CurrentPrimaryDataAssets.Empty(InPrimaryDataAssets.Num());
	PrimaryAssetSet.SetIndex = 0; // Reset the index to 0 when setting new assets

	// TreeView zur�cksetzen
	ClearTreeViewItems();

	for (const UObject* Asset : InPrimaryDataAssets)
	{
		if (Asset)
		{
			PrimaryAssetSet.CurrentPrimaryDataAssets.Add(TSoftObjectPtr<UPrimaryDataAssetSet>(FSoftObjectPath(Asset)));
		}
		else
		{
			UE_LOGFMT(Log_AssetActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: Skipping null asset in SetCurrentPrimaryDataAssets.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
		}
	}

	if (PrimaryAssetSet.CurrentPrimaryDataAssets.Num() > 0)
	{
		UE_LOGFMT(Log_AssetActionsEditorUtilityWidget, Log, "{Time}: {Line} {Class}: Successfully set {Count} primary data assets.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Count", PrimaryAssetSet.CurrentPrimaryDataAssets.Num()),
			("Time", GET_CURRENT_TIME)
		);

		PrimaryAssetSet.OnPrimaryAssetSetsChanged.Broadcast();

		Text_AssetName->SetText(FText::FromString(PrimaryAssetSet.CurrentPrimaryDataAssets[0].GetAssetName()));
		Text_AssetPath->SetText(UEntityCoreEditorFunctionLibrary::GetAssetNameFromInternalNameString(PrimaryAssetSet.CurrentPrimaryDataAssets[0].GetAssetName()));

		SetCurrentAssetView();
	}
	else
	{
		UE_LOGFMT(Log_AssetActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: No valid primary data assets provided.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME)
		);
	}
}

void UAssetActionsEditorUtilityWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	PrimaryAssetSetList->SetObject(this);
}

void UAssetActionsEditorUtilityWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(!SaveAsset->OnClicked.IsBound())
	{
		SaveAsset->OnClicked.AddDynamic(this, &UAssetActionsEditorUtilityWidget::OnSaveAssetClicked);
	}

	if(!ResetAsset->OnClicked.IsBound())
	{
		ResetAsset->OnClicked.AddDynamic(this, &UAssetActionsEditorUtilityWidget::OnResetAssetClicked);
	}

	if(!LoadAllAssets->OnClicked.IsBound())
	{
		LoadAllAssets->OnClicked.AddDynamic(this, &UAssetActionsEditorUtilityWidget::OnLoadAllAssetsClicked);
	}

	// Bind the PrimaryAssetSetChanged event to update the UI when the asset set changes
	if (!PrimaryAssetSet.OnPrimaryAssetSetSelected.IsBoundToObject(this))
	{
		PrimaryAssetSet.OnPrimaryAssetSetSelected.AddUObject(this, &UAssetActionsEditorUtilityWidget::OnPrimaryAssetSetChanged);
	}
}

void UAssetActionsEditorUtilityWidget::OnSaveAssetClicked()
{
	UE_LOGFMT(Log_AssetActionsEditorUtilityWidget, Verbose, "{Time}: {Line} {Class}: Save Asset button clicked.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME)
	);
}

void UAssetActionsEditorUtilityWidget::OnResetAssetClicked()
{
	UE_LOGFMT(Log_AssetActionsEditorUtilityWidget, Verbose, "{Time}: {Line} {Class}: Reset Asset button clicked.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME)
	);

	ClearTreeViewItems();

	SetCurrentAssetView();
}

void UAssetActionsEditorUtilityWidget::OnLoadAllAssetsClicked()
{
	UE_LOGFMT(Log_AssetActionsEditorUtilityWidget, Verbose, "{Time}: {Line} {Class}: Load All Assets button clicked.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME)
	);

	FPrimaryAssetType AssetType = UPrimaryDataAssetSet::GetAssetType();

	TArray<FPrimaryAssetId> AssetIds;
	UAssetManager& AssetManager = UAssetManager::Get();
	AssetManager.GetPrimaryAssetIdList(FPrimaryAssetType(AssetType), AssetIds);

	TArray<TSoftObjectPtr<UPrimaryDataAssetSet>> PrimaryDataAssets;

	for (const FPrimaryAssetId& Id : AssetIds)
	{
		FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(Id);
		UObject* Loaded = AssetPath.TryLoad();

		if (UPrimaryDataAssetSet* Set = Cast<UPrimaryDataAssetSet>(Loaded))
		{
			PrimaryDataAssets.Add(Set);
			UE_LOGFMT(Log_AssetActionsEditorUtilityWidget, Log, "{Time}: {Line} {Class}: Loaded Primary Data Asset Set: {AssetName}",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("AssetName", Set->GetName()),
				("Time", GET_CURRENT_TIME)
			);
		}
		else
		{
			UE_LOGFMT(Log_AssetActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: Failed to load Primary Data Asset Set from path: {Path}",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Path", AssetPath.ToString()),
				("Time", GET_CURRENT_TIME)
			);
		}
	}


	if (PrimaryDataAssets.Num() == 0)
	{
		UE_LOGFMT(Log_AssetActionsEditorUtilityWidget, Warning, "{Time}: {Line} {Class}: No valid primary data assets found.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME)
		);
		return;
	}

	PrimaryAssetSet.CurrentPrimaryDataAssets = PrimaryDataAssets;
	PrimaryAssetSet.SetIndex = 0;

	PrimaryAssetSet.OnPrimaryAssetSetsChanged.Broadcast();

	UE_LOGFMT(Log_AssetActionsEditorUtilityWidget, Display, "{Time}: {Line} {Class}: Updated PrimaryAssets with {Num} PrimaryDataAssetSets",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Num", PrimaryAssetSet.CurrentPrimaryDataAssets.Num()),
		("Time", GET_CURRENT_TIME)
	);
}

void UAssetActionsEditorUtilityWidget::OnPrimaryAssetSetChanged()
{
	UE_LOGFMT(Log_AssetActionsEditorUtilityWidget, Display, "{Time}: {Line} {Class}: Primary Asset Set Changed. Index is {Index}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Index", PrimaryAssetSet.SetIndex)
	);

	ClearTreeViewItems();

	SetCurrentAssetView();
}

void UAssetActionsEditorUtilityWidget::ClearTreeViewItems()
{
	UtilityTreeView_Tool->ClearSelection();
	UtilityTreeView_Tool->ClearListItems();     // TreeView zur�cksetzen
	UtilityTreeView->ClearSelection();
	UtilityTreeView->ClearListItems();
}