// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PreLoadingAssetToolUtilityWidget.h"

#include "Components/Button.h"
#include "Components/DetailsView.h"
#include "Components/TextBlock.h"

#include "DataAssets/PreLoadingDataAsset.h"

#include "Libs/EntityCoreEditorFunctionLibrary.h"

#include "Logging/GlobalLog.h"
#include "Logging/Logging_Editor_VisualEditor.h"
#include "Logging/StructuredLog.h"

#include "EditorUtilityWidgetComponents.h"

UPreLoadingAssetToolUtilityWidget::UPreLoadingAssetToolUtilityWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UPreLoadingAssetToolUtilityWidget::SetPreLoadingDataAsset(UPreLoadingDataAsset* InPreLoadingDataAsset)
{
	if (!PreLoadingDataAsset)
	{
		UE_LOGFMT(Log_PreLoadingAssetTool, Warning, "{Time}: {Line} {Class}: PreLoadingDataAsset is null. Cannot set asset data.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME)
		);
		return;
	}

	PreLoadingDataAsset = InPreLoadingDataAsset;

	Text_AssetName->SetText(FText::FromString(InPreLoadingDataAsset->GetName()));
	Text_AssetPath->SetText(FText::FromString(InPreLoadingDataAsset->GetPathName()));
}

void UPreLoadingAssetToolUtilityWidget::NativePreConstruct()
{
	// This function is called before the widget is constructed, allowing you to set up properties or perform initializations.
	// You can override this function to add custom pre-construction logic.
	Super::NativePreConstruct();

}

void UPreLoadingAssetToolUtilityWidget::NativeConstruct()
{
	// This function is called after the widget is constructed, allowing you to set up bindings or perform additional initializations.
	// You can override this function to add custom construction logic.
	Super::NativeConstruct();


	if (!SaveAsset->OnClicked.IsBound())
	{
		SaveAsset->OnClicked.AddDynamic(this, &UPreLoadingAssetToolUtilityWidget::OnSaveAssetClicked);
	}

	if (!ResetAsset->OnClicked.IsBound())
	{
		ResetAsset->OnClicked.AddDynamic(this, &UPreLoadingAssetToolUtilityWidget::OnResetAssetClicked);
	}

	UE_LOGFMT(Log_PreLoadingAssetTool, Verbose, "{Time}: {Line} {Class}: PreLoadingAssetEditorUserWidget constructed with asset: {Name}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Name", PreLoadingDataAsset ? PreLoadingDataAsset->GetName() : TEXT("null"))
	);
}

void UPreLoadingAssetToolUtilityWidget::NativeDestruct()
{
	// This function is called when the widget is being destroyed, allowing you to clean up resources or perform final actions.
	// You can override this function to add custom destruction logic.
	Super::NativeDestruct();
}

void UPreLoadingAssetToolUtilityWidget::NativeOnItemSelectionChanged(bool bIsSelected)
{
	UE_LOGFMT(Log_PreLoadingAssetTool, Verbose, "{Time}: {Line} {Class}: Item selection changed. IsSelected: {IsSelected}, Asset: {Name}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("IsSelected", bIsSelected ? TEXT("true") : TEXT("false")),
		("Name", PreLoadingDataAsset ? PreLoadingDataAsset->GetName() : TEXT("null"))
	);


}

void UPreLoadingAssetToolUtilityWidget::NativeOnItemExpansionChanged(bool bIsExpanded)
{
	UE_LOGFMT(Log_PreLoadingAssetTool, Verbose, "{Time}: {Line} {Class}: Item expansion changed. IsExpanded: {IsExpanded}, Asset: {Name}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("IsExpanded", bIsExpanded ? TEXT("true") : TEXT("false")),
		("Name", PreLoadingDataAsset ? PreLoadingDataAsset->GetName() : TEXT("null"))
	);
}

void UPreLoadingAssetToolUtilityWidget::NativeOnEntryReleased()
{
	UE_LOGFMT(Log_PreLoadingAssetTool, Verbose, "{Time}: {Line} {Class}: Entry released for asset: {Name}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Name", PreLoadingDataAsset ? PreLoadingDataAsset->GetName() : TEXT("null"))
	);
}

void UPreLoadingAssetToolUtilityWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	PreLoadingDataAsset = Cast<UPreLoadingDataAsset>(ListItemObject);

	if (!PreLoadingDataAsset)
	{
		UE_LOGFMT(Log_PreLoadingAssetTool, Warning, "{Time}: {Line} {Class}: List item object is not a valid PreLoadingDataAsset.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME)
		);
		return;
	}

	Text_AssetName->SetText(FText::FromName(PreLoadingDataAsset->GetFName()));
	Text_AssetPath->SetText(UEntityCoreEditorFunctionLibrary::GetAssetNameFromInternalNameString(PreLoadingDataAsset->GetPathName()));

	UE_LOGFMT(Log_PreLoadingAssetTool, Display, "{Time}: {Line} {Class}: List item object set. Asset: {Name}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Name", ListItemObject ? ListItemObject->GetName() : TEXT("null"))
	);
}

void UPreLoadingAssetToolUtilityWidget::OnSaveAssetClicked()
{
	UE_LOGFMT(Log_PreLoadingAssetTool, Verbose, "{Time}: {Line} {Class}: Save Asset button clicked. {Name}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Name", PreLoadingDataAsset ? PreLoadingDataAsset->GetName() : TEXT("null"))
	);
}

void UPreLoadingAssetToolUtilityWidget::OnResetAssetClicked()
{
	UE_LOGFMT(Log_PreLoadingAssetTool, Verbose, "{Time}: {Line} {Class}: Reset Asset button clicked {Name}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Name", PreLoadingDataAsset ? PreLoadingDataAsset->GetName() : TEXT("null"))
	);
}


