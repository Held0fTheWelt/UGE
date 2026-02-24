// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/Primary/InformativePrimaryDataAsset.h"
#include "PrimaryDataAssetSet.generated.h"

/**
 * 
 */
UCLASS()
class ASSETLOADER_API UPrimaryDataAssetSet : public UInformativePrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPrimaryDataAssetSet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
		: Super(ObjectInitializer)
	{
		AssetTypeName = GetAssetType();
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	FName AssetSetName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	TArray<TSoftObjectPtr<class UPreLoadingDataAsset>> AssetSet;

	/** Returns the static primary asset type for this data asset class */
	static const FPrimaryAssetType GetAssetType()
	{
		return AssetType;
	}

private:
	/** The static asset type for this data asset class */
	static const FPrimaryAssetType AssetType;
};
