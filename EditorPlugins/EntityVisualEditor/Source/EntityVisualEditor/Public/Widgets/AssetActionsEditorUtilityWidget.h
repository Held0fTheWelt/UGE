// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "Interfaces/AssetEditorInterface.h"
#include "Structs/PrimaryAssetSetArray.h"
#include "AssetActionsEditorUtilityWidget.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(Log_AssetActionsEditorUtilityWidget, Log, All);

/**
 * 
 */
UCLASS(BlueprintType)
class ENTITYVISUALEDITOR_API UAssetActionsEditorUtilityWidget : public UEditorUtilityWidget, public IAssetEditorInterface
{
	GENERATED_BODY()
	
public:
	UAssetActionsEditorUtilityWidget();

	virtual void SetCurrentPrimaryDataAssets_Implementation(const TArray<class UObject*>& InPrimaryDataAssets) override;

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text_AssetName;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text_AssetPath;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UEditorUtilityButton* SaveAsset;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UEditorUtilityButton* ResetAsset;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UEditorUtilityButton* LoadAllAssets;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UEditorUtilityTreeView* UtilityTreeView_Tool;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UEditorUtilityTreeView* UtilityTreeView;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UDetailsView* PrimaryAssetSetList;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PrimaryAssets)
	FPrimaryAssetSetArray PrimaryAssetSet;

private:
	UFUNCTION()
	void OnSaveAssetClicked();
	UFUNCTION()
	void OnResetAssetClicked();
	UFUNCTION()
	void OnLoadAllAssetsClicked();
	UFUNCTION()
	void OnPrimaryAssetSetChanged();

	void SetCurrentAssetView();

	void ClearTreeViewItems();
};
