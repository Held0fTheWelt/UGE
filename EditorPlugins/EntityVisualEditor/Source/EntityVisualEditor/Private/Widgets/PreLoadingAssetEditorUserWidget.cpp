// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PreLoadingAssetEditorUserWidget.h"

#include "Components/DetailsView.h"

#include "DataAssets/PreLoadingDataAsset.h"

#include "Libs/EntityCoreEditorFunctionLibrary.h"

#include "Logging/GlobalLog.h"
#include "Logging/Logging_Editor_VisualEditor.h"
#include "Logging/StructuredLog.h"

#include "EditorUtilityWidgetComponents.h"

UPreLoadingAssetEditorUserWidget::UPreLoadingAssetEditorUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UPreLoadingAssetEditorUserWidget::SetPreLoadingDataAsset(UPreLoadingDataAsset* InPreLoadingDataAsset)
{ 
	if(!PreLoadingDataAsset)
	{
		UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: PreLoadingDataAsset is null. Cannot set asset data.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME)
		);
		return;
	}

	PreLoadingDataAsset = InPreLoadingDataAsset; 
	
	SetDetailsViewData();
}

void UPreLoadingAssetEditorUserWidget::NativePreConstruct()
{
	// This function is called before the widget is constructed, allowing you to set up properties or perform initializations.
	// You can override this function to add custom pre-construction logic.
	Super::NativePreConstruct();

	DetailsView->SetObject(this);
}

void UPreLoadingAssetEditorUserWidget::NativeConstruct()
{
	// This function is called after the widget is constructed, allowing you to set up bindings or perform additional initializations.
	// You can override this function to add custom construction logic.
	Super::NativeConstruct();
}

void UPreLoadingAssetEditorUserWidget::NativeDestruct()
{
	// This function is called when the widget is being destroyed, allowing you to clean up resources or perform final actions.
	// You can override this function to add custom destruction logic.
	Super::NativeDestruct();
}

void UPreLoadingAssetEditorUserWidget::NativeOnItemSelectionChanged(bool bIsSelected)
{
	UE_LOGFMT(Log_PreLoadingAssetEditor, Verbose, "{Time}: {Line} {Class}: Item selection changed. IsSelected: {IsSelected}, Asset: {Name}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("IsSelected", bIsSelected ? TEXT("true") : TEXT("false")),
		("Name", PreLoadingDataAsset ? PreLoadingDataAsset->GetName() : TEXT("null"))
	);
}

void UPreLoadingAssetEditorUserWidget::NativeOnItemExpansionChanged(bool bIsExpanded)
{
	UE_LOGFMT(Log_PreLoadingAssetEditor, Verbose, "{Time}: {Line} {Class}: Item expansion changed. IsExpanded: {IsExpanded}, Asset: {Name}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("IsExpanded", bIsExpanded ? TEXT("true") : TEXT("false")),
		("Name", PreLoadingDataAsset ? PreLoadingDataAsset->GetName() : TEXT("null"))
	);
}

void UPreLoadingAssetEditorUserWidget::NativeOnEntryReleased()
{
	UE_LOGFMT(Log_PreLoadingAssetEditor, Verbose, "{Time}: {Line} {Class}: Entry released for asset: {Name}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Name", PreLoadingDataAsset ? PreLoadingDataAsset->GetName() : TEXT("null"))
	);
}

void UPreLoadingAssetEditorUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	PreLoadingDataAsset = Cast<UPreLoadingDataAsset>(ListItemObject);

	if(!PreLoadingDataAsset)
	{
		UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: List item object is not a valid PreLoadingDataAsset.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME)
		);
		return;
	}

	NativeSetDetailsViewData();

	UE_LOGFMT(Log_PreLoadingAssetEditor, Display, "{Time}: {Line} {Class}: List item object set. Asset: {Name}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Name", ListItemObject ? ListItemObject->GetName() : TEXT("null"))
	);
}