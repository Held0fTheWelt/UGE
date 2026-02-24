// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AssetEditorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAssetEditorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ENTITYCOREEDITOR_API IAssetEditorInterface
{
	GENERATED_BODY()

public:
	/**
	 * Sets the current primary data assets to be edited.
	 * @param InPrimaryDataAssets The array of primary data assets to set.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Asset Editor")
	void SetCurrentPrimaryDataAssets(const TArray<class UObject*>& InPrimaryDataAssets);
};
