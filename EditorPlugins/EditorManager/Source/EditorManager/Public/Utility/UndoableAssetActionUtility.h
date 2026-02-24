// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"
#include "UndoableAssetActionUtility.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(Log_UndoableAssetActionUtility, Log, All);

/**
 * 
 */
UCLASS()
class EDITORMANAGER_API UUndoableAssetActionUtility : public UAssetActionUtility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility")
	FString UtilityName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility")
	FString UtilityDescription;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	bool EditAsset(class UObject* Object);
	virtual bool EditAsset_Implementation(class UObject* Object);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	bool EditAllAssets(const TArray<class UObject*>& Objects);
	virtual bool EditAllAssets_Implementation(const TArray<class UObject*>& Objects);
	/**
	 * This function is called when the asset action utility is executed.
	 * It should be overridden to implement the specific functionality of the utility.
	 * UFUNCTION(CallInEditor, Category = "UtilityCategory") 
	 */
	UFUNCTION(BlueprintCallable, Category = "Utility")
	virtual void ExecuteUndoableAction(class UObject* PrimaryObject);
	/**
	 * This function is called to determine if the asset action utility can be executed.
	 * It should return true if the utility can be executed, false otherwise.
	 */
	virtual bool CanExecuteUndoableAction(class UObject* PrimaryObject) const;

protected:
	// The mode of the utility action, can be used to differentiate between different actions or states.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Utility")
	int32 Mode = 0;

	UPROPERTY(Transient)
	class UEditorUtilityTask_PrepareUndo* PrepareUndoTask = nullptr;
};
