// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/UndoableAssetActionUtility.h"
#include "AssetSetActionUtility.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(Log_AssetSetActionUtility, Log, All);

/**
 * 
 */
UCLASS()
class ENTITYCOREEDITOR_API UAssetSetActionUtility : public UUndoableAssetActionUtility
{
	GENERATED_BODY()
	
public:
	UAssetSetActionUtility();

	// Override the EditAsset function to provide custom functionality
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Utility")
	void OpenAssetSetEditor();

private:
	UPROPERTY(Transient)
	FName TabID = FName("None");

private:
	// Override the EditAsset function to provide custom functionality
	virtual bool EditAllAssets_Implementation(const TArray<class UObject*>& Objects) override;

	class UEditorUtilityWidget* RunEditorUtilityWidget(class UEditorUtilityWidgetBlueprint* ActionsWidget);

	class UEditorUtilityWidgetBlueprint* GetAssetActionsEditorUtilityWidget() const;
};
