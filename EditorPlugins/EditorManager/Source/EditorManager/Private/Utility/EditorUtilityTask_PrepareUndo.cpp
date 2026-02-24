// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/EditorUtilityTask_PrepareUndo.h"

#include "Internationalization/Internationalization.h"

#include "Libs/EditorManagerUtilityLibrary.h"

#include "EditorUtilitySubsystem.h"

void UEditorUtilityTask_PrepareUndo::BeginExecution()
{
	Super::BeginExecution();

	bool bWasCancelRequested = WasCancelRequested();

	SetTaskNotificationText(FText::FromString("Preparing Undo Options"));

	TArray<FAssetData> AssetDataArray;
	GEditor->GetContentBrowserSelections(AssetDataArray); // Holt alle selektierten Assets im Content Browser

	TArray<UObject*> SelectedAssets;
	for (const FAssetData& AssetData : AssetDataArray)
	{
		UObject* Asset = AssetData.GetAsset();
		if (Asset)
		{
			SelectedAssets.Add(Asset);
		}
	}

	if(SelectedAssets.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("EditorUtilityTask_PrepareUndo: No assets selected to prepare undo options."));
		FinishExecutingTask(false);
		return;
	}

	for(UObject* Asset : SelectedAssets)
	{
		if(!Asset)
		{
			UE_LOG(LogTemp, Warning, TEXT("EditorUtilityTask_PrepareUndo: Asset is null, skipping."));
			continue;
		}

		if(!Asset->IsValidLowLevel())
		{
			UE_LOG(LogTemp, Warning, TEXT("EditorUtilityTask_PrepareUndo: Asset %s is not valid, skipping."), *Asset->GetName());
			continue;
		}

		if(!Asset->HasAnyFlags(RF_Transactional))
		{
			UE_LOG(LogTemp, Warning, TEXT("EditorUtilityTask_PrepareUndo: Asset %s is not transactional, skipping."), *Asset->GetName());
			continue;
		}

		if(!Asset->IsA<UObject>())
		{
			UE_LOG(LogTemp, Warning, TEXT("EditorUtilityTask_PrepareUndo: Asset %s is not a UObject, skipping."), *Asset->GetName());
			continue;
		}
		
		bWasCancelRequested = WasCancelRequested();

		if(bWasCancelRequested)
		{
			UE_LOG(LogTemp, Warning, TEXT("EditorUtilityTask_PrepareUndo: Cancel requested, stopping execution."));
			continue;
		}

		
	}
	
	if(!bWasCancelRequested)
	{
		SetTaskNotificationText(FText::FromString("Undo Options Prepared"));
		UEditorManagerUtilityLibrary::CheckoutSaveAndEndTransaction(SelectedAssets);
	}
	else
	{
		SetTaskNotificationText(FText::FromString("Undo Preparation Cancelled"));
	}

	FinishExecutingTask(true);
}
