// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/PreLoadingDataAsset.h"
#include "Structs/HelicopterSetupData.h"
#include "HelicopterSetupDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class CORE_DATA_API UHelicopterSetupDataAsset : public UPreLoadingDataAsset
{
	GENERATED_BODY()

public:
	UHelicopterSetupDataAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
		: Super(ObjectInitializer)
	{
		AssetTypeName = GetAssetType_Implementation();
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Setup)
	FHelicopterSetupData HelicopterSetupData;

	/** Returns the static primary asset type for this data asset class */
	virtual const FPrimaryAssetType GetAssetType_Implementation() const override
	{
		return AssetType;
	}

	static FPrimaryAssetType GetAssetType()
	{
		return AssetType;
	}

private:
	/** The static asset type for this data asset class */
	static const FPrimaryAssetType AssetType;
};
