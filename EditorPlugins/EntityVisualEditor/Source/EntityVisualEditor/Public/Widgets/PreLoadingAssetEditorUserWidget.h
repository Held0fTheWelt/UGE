// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "PreLoadingAssetEditorUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ENTITYVISUALEDITOR_API UPreLoadingAssetEditorUserWidget : public UEditorUtilityWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	UPreLoadingAssetEditorUserWidget(const FObjectInitializer& ObjectInitializer);

	void SetPreLoadingDataAsset(class UPreLoadingDataAsset* InPreLoadingDataAsset); 
protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UDetailsView* DetailsView;
protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void SetDetailsViewData();
	void SetDetailsViewData_Implementation() {}

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

	void NativeSetDetailsViewData();
};
