// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "PreLoadingAssetToolUtilityWidget.generated.h"

/**
 * 
 */
UCLASS()
class ENTITYVISUALEDITOR_API UPreLoadingAssetToolUtilityWidget : public UEditorUtilityWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UPreLoadingAssetToolUtilityWidget(const FObjectInitializer& ObjectInitializer);
	
	void SetPreLoadingDataAsset(class UPreLoadingDataAsset* InPreLoadingDataAsset);
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text_AssetName;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text_AssetPath;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UEditorUtilityButton* SaveAsset;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UEditorUtilityButton* ResetAsset;

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;
	virtual void NativeOnItemExpansionChanged(bool bIsExpanded) override;
	virtual void NativeOnEntryReleased() override;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

private:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "PreLoading", meta = (AllowPrivateAccess = "true"))
	class UPreLoadingDataAsset* PreLoadingDataAsset;

private:
	UFUNCTION()
	void OnSaveAssetClicked();
	UFUNCTION()
	void OnResetAssetClicked();
};
