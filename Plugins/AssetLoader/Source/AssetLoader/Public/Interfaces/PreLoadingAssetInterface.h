// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PreLoadingAssetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPreLoadingAssetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ASSETLOADER_API IPreLoadingAssetInterface
{
	GENERATED_BODY()

public:
	/**
	 * Returns the asset type name for this pre-loading asset.
	 * @return The asset type name.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = AssetLoader)
	TArray<TSoftClassPtr<UObject>> GetAssetClasses() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = AssetLoader)
	TArray<TSoftObjectPtr<UObject>> GetAssetObjects() const;
};
