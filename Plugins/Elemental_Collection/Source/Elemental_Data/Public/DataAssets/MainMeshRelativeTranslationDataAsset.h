// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/PreLoadingDataAsset.h"
#include "MainMeshRelativeTranslationDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTAL_DATA_API UMainMeshRelativeTranslationDataAsset : public UPreLoadingDataAsset
{
	GENERATED_BODY()
	
public:
	UMainMeshRelativeTranslationDataAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
		: Super(ObjectInitializer),
		RelativeTranslation(FTransform::Identity)
	{
		AssetTypeName = GetAssetType_Implementation();
	}

	/** The relative translation to apply to the main mesh */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Transform)
	FTransform RelativeTranslation;

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
